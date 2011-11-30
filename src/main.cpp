#include <iostream>
#include <vector>
#include <string>
#include <SDL\SDL.h>
#include <stdlib.h>
#include <time.h>
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


using std::vector;
using std::cout;
using std::endl;
using std::string;


SDL_Surface* surfScreen;

    /*游戏部分*/
void gameInit(SDL_Surface* pScreen);
void gameUpdate();
void gameRender();

     /*控制部分*/
vector<int> command; //hero的命令队列
void onSpaceOrRButton();
void onLeftClick(int mx, int my);

class App: public RPG_EventHandleInterface
{
	public:
	
		void handleKeyState(Uint8 *keystate){
			if(gamePause)return;
			if(keystate[SDLK_UP]){
				command.clear();
				RPG_SpriteManager::getNPC(RPG_SceneManager::getCurrentMap(), 0)->moveUp();
			}
			else if(keystate[SDLK_DOWN]){
				command.clear();
				RPG_SpriteManager::getNPC(RPG_SceneManager::getCurrentMap(), 0)->moveDown();
			} 
			else if(keystate[SDLK_LEFT]){
				command.clear();
				RPG_SpriteManager::getNPC(RPG_SceneManager::getCurrentMap(), 0)->moveLeft();
			}
			else if(keystate[SDLK_RIGHT]){
				command.clear();
				RPG_SpriteManager::getNPC(RPG_SceneManager::getCurrentMap(), 0)->moveRight();
			}
		}
		
		void onKeyDown(SDLKey sym){
			if(gamePause)return;
			if(sym==SDLK_ESCAPE){
				//new RPG_EquipmentSystem(); //调出装备系统
				//new RPG_ShopingSystem(5, 1); //调出商店系统
				//new RPG_TaskSystem(0);
				new RPG_LearnMagic(6, 1);
			}
			else if(sym==SDLK_SPACE){
				onSpaceOrRButton();
			}
		}

		void onLButtonDown(int mx, int my){
			if(gamePause)return;
			onLeftClick(mx, my);
		}

		void onRButtonDown(int mx, int my){
			if(gamePause)return;
			onSpaceOrRButton(); //鼠标右键等同于空格
		}
};

int main(int argc, char *argv[])
{
	
    cout << "starting.." << endl;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDL_WM_SetCaption("Super_RPG", "image\\icon\\H.ICO");
    atexit(SDL_Quit);
	RPG_NS_SoundManager::RPG_SoundManager::disableMusic(true); //禁止背景音乐 
    surfScreen = SDL_SetVideoMode(GAME_WINDOW_W, GAME_WINDOW_H, 0,  SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	//surfScreen = SDL_SetVideoMode(GAME_WINDOW_W, GAME_WINDOW_H, 0,  SDL_SWSURFACE|SDL_DOUBLEBUF);
    RPG_SoundManager::AudioManager_Init();
	try{
		gameInit(surfScreen); //游戏初始化
		App app; //玩家控制器
		RPG_EventManager::addListen(&app, RPG_EventManager::EVENTMANAGER_HANDLEKEYSTATE);
		RPG_EventManager::addListen(&app, RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
		RPG_EventManager::addListen(&app, RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
		RPG_EventManager::addListen(&app, RPG_EventManager::EVENTMANAGER_ONRBUTTONDOWN);

		Uint32 lastTime1, lastTime2;
		lastTime1 = lastTime2 = SDL_GetTicks();
        while(gameOver==false)
        {
			if(SDL_GetTicks()-lastTime1>10){
				//控制
				lastTime1 = SDL_GetTicks();
				RPG_EventManager::listen();
			}
			if(SDL_GetTicks()-lastTime2>20){
				//更新与渲染
				lastTime2 = SDL_GetTicks();	
				gameUpdate();
				gameRender();
				
			}
        }
    }
    catch (const string& err){
        cout << err << endl;
    }
    return 0;
}

void gameInit(SDL_Surface* pScreen){

/****************模拟载入的玩家信息****************/
	heroSet.heroCount = 1;
	
	heroSet.heros[0].attribute.zdxl = 100;
	heroSet.heros[0].attribute.zdml = 100;
	strcpy(heroSet.heros[0].heroName,"刘天凤");
	heroSet.heros[0].blood = 100;
	heroSet.heros[0].mana = 50;
	heroSet.heros[0].cp = 60;
	heroSet.heros[0].pointSoul = 0;
	Mate t;
	strcpy(t.material, "香");
	t.num = 1;
	//heroPackage.drug.push_back(t);

	/*heroSet.heros[1].attribute.zdxl = 130;
	heroSet.heros[1].attribute.zdml = 80;
	strcpy(heroSet.heros[1].heroName, "云千雪");
	heroSet.heros[1].pointSoul = 9;

	heroSet.heros[2].attribute.zdxl = 130;
	heroSet.heros[2].attribute.zdml = 80;
	strcpy(heroSet.heros[2].heroName, "南宫忆柔");
	heroSet.heros[2].pointSoul = 10;*/

	//Mate t;
	strcpy(t.material, "铠甲");
	t.num = 1;
	heroPackage.weapon.push_back(t);
	heroSet.items[0].push_back(t);
	//heroSet.items[1].push_back(t);

	strcpy(t.material, "铁剑");
	t.num = 1;
	heroPackage.weapon.push_back(t);
	heroSet.items[0].push_back(t);

	strcpy(t.material, "小血瓶");
	t.num = 5;
	heroPackage.drug.push_back(t);
	
	strcpy(t.material, "魔瓶");
	t.num = 3;
	heroPackage.drug.push_back(t);

	strcpy(t.material, "苏醒药");
	t.num = 2;
	heroPackage.drug.push_back(t);

	heroPackage.finance = 0;
	/****************模拟载入任务信息***********************/
	RPG_TaskSystem::RPG_TaskSystem_Init(); //任务系统初始化

	gameChapter = 1; //根据章节剧情机可以选择相应剧情
	//RPG_TaskSystem::addChapter(); //新增第一章
	/*RPG_TaskSystem::addChapter(); //新增第二章
	RPG_TaskSystem::receive("chapter1_main_1");
	RPG_TaskSystem::receive("chapter1_branch_1");
	RPG_TaskSystem::receive("chapter1_branch_2");
	RPG_TaskSystem::complete("chapter1_branch_1");
	RPG_TaskSystem::receive("chapter2_main_1");*/



	srand(time(NULL));
	RPG_SceneManager::RPG_SceneManager_Init("map\\world0.map", "image\\tiled\\tiled0.bmp", 8, 8, pScreen);
	RPG_AI::RPG_AI_Init(); // AI系统初始化
	RPG_SpriteManager::init();
	
	gamePause = false;
	gameOver = false;

}

void gameUpdate(){
	if(gamePause)return;

	story_tot(); //剧情机

	/**********************/
	RPG_NPC* npc = RPG_SpriteManager::getNPC(RPG_SceneManager::getCurrentMap(), 0);
	if(!npc->isBusy() && command.size()>0 ){ //如果不忙且有命令则可以接受命令
		vector<int>::iterator iter = command.begin();
		npc->move((*iter)); //执行队列首命令
		iter = command.erase(iter); //删除该命令
	}
	RPG_SpriteManager::detectCollision(); //子画面碰撞检测
	RPG_SpriteManager::updateSprites(); //更新子画面
	
	if(RPG_SpriteManager::g_collisionList.size()>0)command.clear();
	/**********************/
	/******跟NPC发生说话碰撞*******/
	if(RPG_SpriteManager::g_collisionList_saying.size()>0){
		RPG_NPC* npc = RPG_SpriteManager::g_collisionList_saying[0]; //说话碰撞只有一个人
		if((npc->getNPCID() & NPC_SHOPER) == NPC_SHOPER){
			new RPG_ShopingSystem(npc->getMapID(), npc->getNPCUnicode());	
		}else if((npc->getNPCID() & NPC_TEACHER) == NPC_TEACHER){
			new RPG_LearnMagic(npc->getMapID(), npc->getNPCUnicode());
		}else{
			RPG_DialogSystem::createDialog(npc->getMapID(), npc->getNPCUnicode());
		}
	}

	/**********************/
	RPG_SceneManager::On_detectChange();//检查是否需要切换地图
	RPG_AI::AIMove();

	RPG_SpriteManager::g_collisionList_saying.clear();
}

void gameRender(){
	RPG_SceneManager::RPG_GameRender(); //渲染
	RPG_ShopingSystem::render(surfScreen);
	RPG_EquipmentSystem::render(surfScreen);
	RPG_TaskSystem::render(surfScreen);
	RPG_LearnMagic::render(surfScreen);
	SDL_Flip(surfScreen);
}

void onSpaceOrRButton(){
	if(gamePause)return;

	
}

void onLeftClick(int mx, int my){
	if(gamePause)return;
	//寻路
	RPG_NPC* npc = RPG_SpriteManager::getNPC(RPG_SceneManager::getCurrentMap(), 0);
	int sx = npc->getPosX();
	int sy = npc->getPosY(); //获得hero的所在位置
	int ex = RPG_SceneManager::convertCoordinate(mx, my).posX;
	int ey = RPG_SceneManager::convertCoordinate(mx, my).posY; //将屏幕坐标转换为地图瓷砖坐标
	FindPath::findPath(RPG_SceneManager::getMap(), sx, sy, ex, ey, &command);
}
