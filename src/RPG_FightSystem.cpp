#include "../include/RPG_FightSystem.h"
#include "../include/RPG_DialogSystem.h"
#include "../include/RPG_SceneManager.h"

/*	npcIDtrans.dat 文件格式
	npcID(外部系统的npc ID) num(战斗触发后的npc数量) fID0 fID1 .....
	...
	-1
*/

RPG_NS_FightSys::RPG_FightSystem::RPG_FightSystem(int npcID, int npcUni){
	m_numOfP = 0;
	m_area.x = 10;//设置战斗区域
	m_area.y = 10;
	m_area.w = 10;
	m_area.h = 10;
	Internal_init();
	loadHero();
	loadNPC(npcID, npcUni);
	loadMagic();
	m_pScreen = RPG_NS_SceneManager::RPG_SceneManager::getScreenbuf();
}

RPG_NS_FightSys::RPG_FightSystem::~RPG_FightSystem(void){
	delete m_map;
}

void RPG_NS_FightSys::RPG_FightSystem::RPG_runFightSys(int npcID, int npcUni){
	RPG_FightSystem fightSys(npcID, npcUni);
	//
	fightSys.fightCycle();
}

void RPG_NS_FightSys::RPG_FightSystem::loadHero(){
	
	Hero tHero;
	for(int numOfhero=0; numOfhero<heroSet.heroCount; numOfhero++){
		memset(&tHero, 0, sizeof(Hero));
		tHero.attacklength = 3;//这边还得加入装备影响
		tHero.movelength = 5;
		tHero.fid = numOfhero;
		tHero.state = heroSet.heros[numOfhero];
		hero_list.push_back(tHero);
	}
}

void RPG_NS_FightSys::RPG_FightSystem::loadNPC(int npcID, int npcUni){
	//载入战斗所需npc
	int ID, num, fID;
	bool flg;
	FILE *fileTrans;
	FILE *fileNPC;
	NPC tNpc;
	fileTrans = fopen("data\\fight\\npc\\npcIDtrans.dat", "r");
	if(fileTrans == NULL)
		throw "can't find file data\\fight\\npc\\npcIDtrans.dat";
	flg = false;
	while(true){
		fscanf(fileTrans, "%d", &ID);
		if(ID == -1) break;
		fscanf(fileTrans, "%d", &num);
		for(int index=0; index<num; index++){
			string str;
			fscanf(fileTrans, "%d", &fID);
			if(ID == npcID){
				flg = true;
				str = "data\\fight\\npc\\fight_npc"
					+ RPG_NS_DialogSystem::RPG_DialogSystem::intostring(fID) + ".dat"; 
				fileNPC = fopen(str.c_str(), "rb");
				if(fileNPC == NULL)
					throw "can't find" + str;
				fread(&tNpc, sizeof(NPC), 1, fileNPC);
				tNpc.fid = (m_numOfP++) + heroSet.heroCount;//设置战斗ID
				npc_list.push_back(tNpc);//
				fclose(fileNPC);
			}
		}
	}
	fclose(fileTrans);
	if(!flg){
		string str;
		str = "can't find npcid=" + RPG_NS_DialogSystem::RPG_DialogSystem::intostring(npcID)
			+ "in file: npcIDtrans.dat";
		return;
	}
}

void RPG_NS_FightSys::RPG_FightSystem::loadMagic(){

	FILE *fileMagic;
	Magic tmagic;
	fileMagic = fopen("data\\fight\\magic\\magic_list.dat", "rb");
	if(fileMagic == NULL){
		throw std::string("can't find file") + std::string("data\\fight\\magic\\magic_list.dat");
	}
	while(fread(&tmagic, sizeof(Magic), 1, fileMagic)){
		magic_list.push_back(tmagic);
	}
	fclose(fileMagic);
}

void RPG_NS_FightSys::RPG_FightSystem::initMap(){

	m_map = new RPG_NS_Map::RPG_Map("data\\fight\\map\\fightScene.map", false);//只使用内部的Lobic层
	
	for(int i=0; i<heroSet.heroCount; i++){//添加英雄到Npc显示队列
		string npcname = "image\\npc\\npc" + RPG_NS_DialogSystem::RPG_DialogSystem::intostring(i) + ".bmp";
		RPG_NS_NPC::RPG_NPC tNpc(npcname, m_map);
		//分配位置
		tNpc.setPos(m_area.x+m_area.w, m_area.y+i);
		tNpc.setPosE((m_area.x+m_area.w)*m_map->getTiledSize().width, (m_area.y+i)*m_map->getTiledSize().height);
		npclist.push_back(tNpc);
		npcATtime.push_back(0);
	}
	
	for(int i=0; i<npc_list.size(); i++){
		string npcname = "image\\npc\\npc" + RPG_NS_DialogSystem::RPG_DialogSystem::intostring(npc_list[i].fid) + ".bmp";
		RPG_NS_NPC::RPG_NPC tNpc(npcname, m_map);
		//分配位置
		tNpc.setPos(m_area.x, m_area.y+i);
		tNpc.setPosE(m_area.x*m_map->getTiledSize().width, (m_area.y+i)*m_map->getTiledSize().height);
		npclist.push_back(tNpc);
		npcATtime.push_back(0);
	}

}

void RPG_NS_FightSys::RPG_FightSystem::Internal_init(){

	action_sel = -1;
	current_npc = -1;
	fight_over = false;
	m_btnMove = NULL;
	m_btnAttack = NULL;
	m_btnMagic = NULL;
	m_btnMedicine = NULL;
}

void RPG_NS_FightSys::RPG_FightSystem::fightCycle(){

	//战斗系统主循环
	while(!fight_over){
		current_npc = ATreach();
		if(current_npc>=0 && current_npc<heroSet.heroCount){
			
		}
	}
	RPG_NS_EventManager::RPG_EventManager::listen();
}

int RPG_NS_FightSys::RPG_FightSystem::ATreach(){

	if(current_npc != -1)
		return current_npc;
	for(int i=0; i<npcATtime.size(); i++){
		if(npcATtime[i] >= MAX_ATTIME){
			npcATtime[i] = 0;
			return i;//npc i已经到达
		}
	}
	//调整各Npc AT时间
	for(int i=0; i<heroSet.heroCount; i++){
		if(hero_list[i].buf.speed + hero_list[i].state.attribute.sudu > 0){
			npcATtime[i] += hero_list[i].state.attribute.sudu + hero_list[i].buf.speed;
			if(npcATtime[i] > MAX_ATTIME)
				npcATtime[i] = MAX_ATTIME;
		}
	}
	for(int i=0; i<npc_list.size(); i++){
		if(npc_list[i].speed + npc_list[i].buf.speed > 0){
			npcATtime[i] += npc_list[i].speed + npc_list[i].buf.speed;
			if(npcATtime[i] > MAX_ATTIME)
				npcATtime[i] = MAX_ATTIME;
		}
	}
	return -1;
}

void RPG_NS_FightSys::RPG_FightSystem::playerCtl(){

	if(m_btnMove == NULL){
		m_btnMove = new GUI::GUIButton("image\\fight\\btn_移动.bmp", "image\\fight\\btn_移动.bmp",
			700, 400, this);
		m_btnMove->setfClickEvent(move_Helper);
		//
	}
}

void RPG_NS_FightSys::RPG_FightSystem::onMove(){
	
	if(action_sel == MOVE) return;
	action_sel = MOVE;
	waitMove = true;//等待玩家点击移动目的地

}

void RPG_NS_FightSys::RPG_FightSystem::onLButtonDown(int  mx, int my){

	mx = mx/m_map->getTiledSize().width;
	my = my/m_map->getTiledSize().height;
	if(mx<m_area.x || mx>m_area.x+m_area.w || my<m_area.y || my>m_area.y+m_area.h)//点击区域非法
		return;
	
}

void RPG_NS_FightSys::RPG_FightSystem::onRButtonDown(int  mx, int my){

}