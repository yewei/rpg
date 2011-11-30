#include "../include/game.h"
#include "../include/RPG_Config.h"
#include "../include/RPG_NPC.h"
#include "../include/RPG_EventManager.h"
#include "../include/RPG_SceneManager.h"
#include "../include/RPG_SpriteManager.h"
#include "../include/RPG_FindPath.h"
#include "../include/RPG_EquipmentSystem.h"
#include "../include/RPG_ShopingSystem.h"
#include "../include/RPG_DialogSystem.h"
#include "../include/RPG_SoundManager.h"
#include "../include/RPG_AI.h"
#include "../include/RPG_TaskSystem.h"
#include "../include/RPG_LearnMagic.h"

using RPG_NS_NPC::RPG_NPC;
using RPG_NS_EventManager::RPG_EventHandleInterface;
using RPG_NS_EventManager::RPG_EventManager;
using RPG_NS_SceneManager::RPG_SceneManager;
using RPG_NS_SpriteManager::RPG_SpriteManager;
using RPG_NS_FindPath::FindPath;
using RPG_NS_ShopingSystem::RPG_ShopingSystem;
using RPG_NS_EquipmentSystem::RPG_EquipmentSystem;
using RPG_NS_DialogSystem::RPG_DialogSystem;
using RPG_NS_SoundManager::RPG_SoundManager;
using RPG_NS_AI::RPG_AI;
using RPG_NS_TaskSystem::RPG_TaskSystem;
using RPG_NS_LearnMagic::RPG_LearnMagic;

map<string, int> sema; //剧情信号量集


void story_chapter1_main(){
	if( sema["chapter1_begin"]==0 ){
		sema["chapter1_begin"] = 1;
		RPG_TaskSystem::addChapter(); //新增该章的任务界面

		RPG_SpriteManager::addNPC(NPC_HERO1, 20001, 20001, 5, 6);
		RPG_NPC* npc = RPG_SpriteManager::getNPC(20001, 20001);
		npc->setMoveStatus(RPG_NPC::NPC_RIGHT); //朝右

		RPG_DialogSystem::createDialog(0, 0);
	}
	if( sema["wait_maixiang"]==0 ){//买香的任务没有触发，一直检测
		if(RPG_SpriteManager::g_collisionList_saying.size()>0){
			RPG_NPC* npc = RPG_SpriteManager::g_collisionList_saying[0];
			if(npc->getMapID()==20001 && npc->getNPCUnicode()==20001 ){ //跟刘倩柔说话了
				sema["wait_maixiang"] = 1; //接到买香任务
				RPG_TaskSystem::receive("chapter1_main_1");
				RPG_NS_SoundManager::RPG_SoundManager::disableMusic(false);
				RPG_NS_SoundManager::RPG_SoundManager::playSnd(0);//播放该地图音乐 
				heroPackage.finance += 10; //给10块钱买香
				RPG_DialogSystem::createDialog(20001, 20001);
				RPG_SpriteManager::g_collisionList_saying.clear();
				npc->setNPCUnicode(20002);
				//npc = RPG_SpriteManager::getNPC(20002, 1); //杂货店老板娘
				//npc->setNPCUnicode(20001); 

			}
		}
	}
	if( sema["wait_maixiang"]==1 && sema["have_maixiang"]==0 ){
		if(RPG_SpriteManager::g_collisionList_saying.size()>0){ //有人说话
			RPG_NPC* npc = RPG_SpriteManager::g_collisionList_saying[0];
			if(npc->getMapID()==20002 && npc->getNPCUnicode()==1){ //老板娘
				if(sema["laobanniang"]==0){
					sema["laobanniang"] = 1;
					RPG_DialogSystem::createDialog(20002, 20001);
					RPG_SpriteManager::g_collisionList_saying.clear();
				}
			}
		}
		if(hasGood("香")){
			sema["have_maixiang"] = 1; //已经买到香
			RPG_NPC* npc = RPG_SpriteManager::getNPC(20001, 20002);
			npc->setNPCUnicode(20003);
		}
	}
	if( sema["have_maixiang"]==1 &&  sema["alldone_maixiang"]==0 ){
		if(RPG_SpriteManager::g_collisionList_saying.size()>0){
			RPG_NPC* npc = RPG_SpriteManager::g_collisionList_saying[0];
			if(npc->getMapID()==20001 && npc->getNPCUnicode()==20003 ){ //跟刘倩柔说话了
				sema["alldone_maixiang"] = 1;
				RPG_DialogSystem::createDialog(20001, 20003);
				RPG_SpriteManager::g_collisionList_saying.clear();
				RPG_TaskSystem::complete("chapter1_main_1");
				RPG_TaskSystem::receive("chapter1_main_2");
			}
		}
	}
	else if( sema["alldone_maixiang"]==1 && sema["liu_in"]==0 ){
		sema["liu_in"] = 1;
		//刘倩柔加入队伍
		RPG_SpriteManager::killNPCByIDs(20001, 20003);
		heroSet.heroCount = 2;
		heroSet.heros[1].attribute.zdxl = 60;
		heroSet.heros[1].attribute.zdml = 80;
		strcpy(heroSet.heros[1].heroName, "刘倩柔");
		heroSet.heros[1].pointSoul = 0;

	}
	
}

void story_chapter1_branch(){
}

void story_tot(){
	if(gamePause)return;
	switch(gameChapter){
		case 1:
			story_chapter1_main();
			story_chapter1_branch();
			break;
	}
}


