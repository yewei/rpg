#include <stdlib.h>
#include "../include/RPG_AI.h"
#include "../include/RPG_Config.h"
#include "../include/RPG_SceneManager.h"
#include "../include/RPG_FindPath.h"
#include "../include/RPG_Map.h"

vector<RPG_NS_AI::NPCAction> RPG_NS_AI::RPG_AI::m_npcActionList;
vector<RPG_NS_AI::AIItem> RPG_NS_AI::RPG_AI::m_aiItem;

void RPG_NS_AI::RPG_AI::RPG_AI_Init(){
	RPG_NS_AI::NPCAction t;
	int npcId, nCount, moveId;
	FILE* fp = fopen("data\\ai\\npcmove\\all.npcmove", "r");
	if(fp==NULL)throw "打不开文件：data\\ai\\npcmove\\all.npcmove";
	fscanf(fp, "%d", &npcId);
	while(npcId!=-1){
		t.npcId = npcId;
		t.actionLength = 0;
		t.huntLength = -1;
		fscanf(fp, "%d", &t.length); //运动势力范围
		fscanf(fp, "%d", &nCount); //运动类型种类
		while(nCount--){
			fscanf(fp, "%d", &moveId);
			if(moveId==1){ //追赶
				fscanf(fp, "%d", &t.huntLength); //追赶范围
			}
			else{
				t.actionType[t.actionLength++] = moveId;
			}
		}
		m_npcActionList.push_back(t);
		fscanf(fp, "%d", &npcId);
	}

	fclose(fp);
}

void RPG_NS_AI::RPG_AI::updateList(){
	vector<RPG_NPC*>* npclist = RPG_SpriteManager::getSpriteList(); //子画面队列
	RPG_NS_AI::AIItem t;
	for(int i=0; i<npclist->size(); i++){
		if((*npclist)[i]->isDead())continue; //死亡
		if((*npclist)[i]->getNPCID()==NPC_HERO)continue; //英雄不需要AI
		int j;
		for(j=0; j<m_aiItem.size(); j++){//寻找是否存在
			if(m_aiItem[j].mapId==(*npclist)[i]->getMapID()
				&&m_aiItem[j].npcUnicode==(*npclist)[i]->getNPCUnicode()){
				m_aiItem[j].nowX = (*npclist)[i]->getPosX();
				m_aiItem[j].nowY = (*npclist)[i]->getPosY();//更新位置
				break;
			}	
		}
		if(j>=m_aiItem.size()){ //不存在，添加
			t.mapId = (*npclist)[i]->getMapID();
			t.npcId= (*npclist)[i]->getNPCID();
			t.npcUnicode = (*npclist)[i]->getNPCUnicode();
			t.orgX = (*npclist)[i]->getPosX();
			t.orgY = (*npclist)[i]->getPosY();
			t.nowX = t.orgX;
			t.nowY = t.orgY;
			t.npc = (*npclist)[i];
			t.inHunt = false;
			t.goHome = false;
			m_aiItem.push_back(t);
		}
	}
	vector<AIItem>::iterator iter = m_aiItem.begin();
	for(; iter!=m_aiItem.end();){
		int i;
		for(i=0; i<(*npclist).size(); i++){//寻找是否需要删除
			if((*npclist)[i]->getMapID()==(*iter).mapId
				&& (*npclist)[i]->getNPCUnicode()==(*iter).npcUnicode){
				break;
			}
		}
		if(i>=(*npclist).size()){//没找到，需要删除
			iter = m_aiItem.erase(iter);
		}else iter++;
	}
}

RPG_NS_AI::NPCAction RPG_NS_AI::RPG_AI::getActionInfo(int npcId){
	NPCAction t;
	t.npcId = -1; //找不到的标记
	for(int i=0; i<m_npcActionList.size(); i++){
		if(m_npcActionList[i].npcId==npcId)
			return m_npcActionList[i];
	}
	return t;
}

void RPG_NS_AI::RPG_AI::AIMove(){
	int currentMapId;
	currentMapId = RPG_NS_SceneManager::RPG_SceneManager::getCurrentMap(); //当前MapID
	RPG_NS_Map::RPG_Map* map;
	map = RPG_NS_SceneManager::RPG_SceneManager::getMap();

	/***************更新***************/
	updateList(); 

	/*************产生行为*************/
	NPCAction t;
	for(int i=0; i<m_aiItem.size(); i++){
		if(m_aiItem[i].goHome)continue; 
		t = getActionInfo(m_aiItem[i].npcId);
		if(t.npcId == -1)continue; //不需要AI的npc
		if( (abs(m_aiItem[i].nowX-m_aiItem[i].orgX)
			+abs(m_aiItem[i].nowY-m_aiItem[i].orgY)>t.length) 
			&& m_aiItem[i].inHunt==false){//不是追击，超出势力范围
				//回到起点
				m_aiItem[i].actionList.clear();
				RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
					m_aiItem[i].orgX, m_aiItem[i].orgY, &m_aiItem[i].actionList);//A*寻路
				m_aiItem[i].goHome = true;
				continue;
		}
		if( m_aiItem[i].inHunt 
			&& (abs(m_aiItem[i].nowX-m_aiItem[i].orgX)
			+abs(m_aiItem[i].nowY-m_aiItem[i].orgY)>t.huntLength) ){
				//追击时超过追击范围，回到起点
				m_aiItem[i].actionList.clear();
				RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
					m_aiItem[i].orgX, m_aiItem[i].orgY, &m_aiItem[i].actionList);//A*寻路
				m_aiItem[i].goHome = true;
				continue;
		}
		if(m_aiItem[i].inHunt)continue; //如果当前处于追击状态，则不采取别的行为
		if(t.huntLength!=-1){//该类型会追击
			//查看弱碰撞队列,决定是否追击
			int j;
			for(j=0; j<RPG_SpriteManager::g_collisionList_non.size(); j++){
				if(RPG_SpriteManager::g_collisionList_non[j]->getMapID()==m_aiItem[i].mapId
					&& RPG_SpriteManager::g_collisionList_non[j]->getNPCUnicode()==m_aiItem[i].npcUnicode){
					break;
				}
			}
			if(j<RPG_SpriteManager::g_collisionList_non.size()){
				//发生弱碰撞，需要追击
				m_aiItem[i].actionList.clear(); //开始追击时取消所有其他行为
				RPG_NPC* hero = RPG_SpriteManager::getNPC(currentMapId, 0); //英雄
				RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
					hero->getPosX(), hero->getPosY(), &m_aiItem[i].actionList);//A*寻路
				m_aiItem[i].inHunt = true; //置追击动作
				m_aiItem[i].npc->setMoveSpeed(1);
				continue;
			}
		}
		//追击外的其他行为
		if(m_aiItem[i].actionList.size()>0)continue; //其他行为在执行
		if(t.actionLength==0)continue; //无其他动作定义
		int k = rand()%t.actionLength; //随机产生一个动作
		int l, xl, yl , u;
		switch(t.actionType[k])
		{
		case 0: //随机走动
			l = rand()%t.length+1;
			xl = rand()%l+1;
			yl = l - xl;
			if(rand()%2==0)xl = -xl;
			if(rand()%2==0)yl = -yl;
			RPG_NS_FindPath::FindPath::findPath(map, m_aiItem[i].nowX, m_aiItem[i].nowY,
				m_aiItem[i].orgX+xl, m_aiItem[i].orgY+yl, &m_aiItem[i].actionList);//A*寻路
			break;
		case 2: //横向走动
			u = rand()%2; //随机左右
			if(u==0)for(int j=0; j<5; j++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_LEFT);
			else for(int j=0; j<5; j++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_RIGHT);
			break;
		case 3: //纵向走动
			int u = rand()%2; //随机上下
			if(u==0)for(int i=0; i<5; i++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_UP);
			else for(int i=0; i<5; i++)
				m_aiItem[i].actionList.push_back(RPG_NPC::NPC_DOWN);
			break;
		}
	}

	/*****************执行动作**************/
	for(int i=0; i<m_aiItem.size(); i++){
		if(m_aiItem[i].mapId!=currentMapId)continue; //只有当前地图的NPC才处理
		if(m_aiItem[i].npc->isBusy())continue;
		if(m_aiItem[i].actionList.size()==0)continue;
		vector<int>::iterator iter = m_aiItem[i].actionList.begin();
		m_aiItem[i].npc->move((*iter));
		iter = m_aiItem[i].actionList.erase(iter); //删除该动作
		if(m_aiItem[i].actionList.size()==0){
			m_aiItem[i].inHunt = false;
			m_aiItem[i].goHome = false;
			m_aiItem[i].npc->setMoveSpeed(2);
		}
	}
}

