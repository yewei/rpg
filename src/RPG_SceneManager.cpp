#include "../include/RPG_SceneManager.h"
#include "../include/RPG_DialogSystem.h"
#include "../include/RPG_Config.h"
#include "../include/time.h"
#include "../include/RPG_SoundManager.h"
#include "../include/RPG_MenuSurface.h"
#include "../include/RPG_TaskSystem.h"
#include "../include/RPG_EquipmentSystem.h"
#include <sstream>
#include <iostream>

using std::stringstream;

int RPG_NS_SceneManager::RPG_SceneManager::m_currentMap;
RPG_NS_Map::RPG_Map* RPG_NS_SceneManager::RPG_SceneManager::m_map;
RPG_NS_MapRender::RPG_MapRender* RPG_NS_SceneManager::RPG_SceneManager::m_mapRender;
RPG_NS_RenderEngine::RPG_RenderEngine* RPG_NS_SceneManager::RPG_SceneManager::m_renderEngine;
bool RPG_NS_SceneManager::RPG_SceneManager::m_SMisExist;
bool RPG_NS_SceneManager::RPG_SceneManager::bChange;
std::map<int, bool> RPG_NS_SceneManager::RPG_SceneManager::m_mapFlag;
SDL_Rect RPG_NS_SceneManager::RPG_SceneManager::m_gameWindow;
SDL_Surface* RPG_NS_SceneManager::RPG_SceneManager::m_pScreen;
bool RPG_NS_SceneManager::RPG_SceneManager::m_ableChange;
int RPG_NS_SceneManager::RPG_SceneManager::m_climateType = RPG_NS_SceneManager::RPG_SceneManager::SUN;
int RPG_NS_SceneManager::RPG_SceneManager::m_mapNameIndex = 0;
GUI::GUILabel* RPG_NS_SceneManager::RPG_SceneManager::m_mapName = NULL;
GUI::GUILabel* RPG_NS_SceneManager::RPG_SceneManager::m_mapName2 = NULL;
string RPG_NS_SceneManager::RPG_SceneManager::oldmapname = "";
RPG_NS_Image::RPG_Image* RPG_NS_SceneManager::RPG_SceneManager::m_smallMap = NULL;

void RPG_NS_SceneManager::RPG_SceneManager::RPG_SceneManager_Init(std::string mapName, std::string tiledName,
																  int hero_X, int hero_Y, SDL_Surface* pScreen){
	
	srand(time(NULL));
	m_ableChange = true;
	m_pScreen = pScreen;
	changeMap(mapName, tiledName, hero_X, hero_Y);
	//创建小地图
	m_smallMap = new RPG_NS_Image::RPG_Image("image\\gui\\surface\\map_pos.bmp");
	m_smallMap->setColorKey(255, 0, 255);
	RPG_NS_MenuSurface::RPG_MenuSurface::RPG_MenuSurface_Init();//初始化菜单界面

}

void RPG_NS_SceneManager::RPG_SceneManager::changeMap(std::string mapName, std::string tiledName,
												int hero_X, int hero_Y){
	
	RPG_NS_Map::RectType visibleWindow;	//视窗
	int mapID_old, mapID_new;
	
	if(m_mapRender != NULL){ 
	
		mapID_old = m_mapRender->getMap()->getMapID();
		try{
			m_map = new RPG_NS_Map::RPG_Map(mapName.c_str(), true);//生成地图类
		}
		catch(const string &err){
			cout << err << endl;
		}
		mapID_new = m_map->getMapID();
		m_currentMap = mapID_new;
		if(mapID_old<MAP_BASE && mapID_new<MAP_BASE){//大地图之间切换
			RPG_NS_SpriteManager::RPG_SpriteManager::setCurrentMap(m_map);
			RPG_NS_SpriteManager::RPG_SpriteManager::killAllNPC();//清楚NPC队列
			RPG_NS_SpriteManager::RPG_SpriteManager::loadNPC(m_map);	//加载NPC
			RPG_NS_SpriteManager::RPG_SpriteManager::setMap(mapID_new, m_map);//更新指针
			setCurrentMap(mapID_new);
			m_mapFlag.clear();//清空map标记
			m_mapFlag[mapID_new] = true;
		}
		else if(isLoadNPC(mapID_new)){
			setCurrentMap(mapID_new);
			RPG_NS_SpriteManager::RPG_SpriteManager::loadNPC(m_map);	//加载NPC
			RPG_NS_SpriteManager::RPG_SpriteManager::setMap(mapID_new, m_map);//更新指针
			RPG_NS_SpriteManager::RPG_SpriteManager::setCurrentMap(m_map);
			RPG_NS_SpriteManager::RPG_SpriteManager::killNPCByIDs(mapID_old, NPC_HERO);
		}
		else{
			RPG_NS_SpriteManager::RPG_SpriteManager::killNPCByIDs(mapID_old, NPC_HERO);
			RPG_NS_SpriteManager::RPG_SpriteManager::setMap(mapID_new, m_map);//更新指针
			RPG_NS_SpriteManager::RPG_SpriteManager::setCurrentMap(m_map);
		}
	}
	else{				//未加载过地图
		//初始化游戏场景界面
		m_gameWindow.x = m_gameWindow.y = 0;
		m_gameWindow.w = GAME_WINDOW_W;
		m_gameWindow.h = GAME_WINDOW_H; 
		
		//
		mapID_old = 0;
		m_map = new RPG_NS_Map::RPG_Map(mapName.c_str(), true);//生成地图类
		mapID_new = m_map->getMapID();
		m_currentMap = mapID_new;
		if(mapID_old<MAP_BASE && mapID_new<MAP_BASE){
			setCurrentMap(mapID_new);
			RPG_NS_SpriteManager::RPG_SpriteManager::setCurrentMap(m_map);
			RPG_NS_SpriteManager::RPG_SpriteManager::killAllNPC();
			RPG_NS_SpriteManager::RPG_SpriteManager::loadNPC(m_map);
			m_mapFlag.clear();//
			m_mapFlag[mapID_new] = true;
		}
		else if(isLoadNPC(mapID_new)){
			setCurrentMap(mapID_new);
			RPG_NS_SpriteManager::RPG_SpriteManager::loadNPC(m_map);	//加载NPC
			RPG_NS_SpriteManager::RPG_SpriteManager::setMap(mapID_new, m_map);//更新指针
			RPG_NS_SpriteManager::RPG_SpriteManager::setCurrentMap(m_map);
			RPG_NS_SpriteManager::RPG_SpriteManager::killNPCByIDs(mapID_old, NPC_HERO);
		}
		else{
			RPG_NS_SpriteManager::RPG_SpriteManager::killNPCByIDs(mapID_old, NPC_HERO);
			RPG_NS_SpriteManager::RPG_SpriteManager::setMap(mapID_new, m_map);//更新指针
			RPG_NS_SpriteManager::RPG_SpriteManager::setCurrentMap(m_map);
		}
	}
	
	m_mapRender = new RPG_NS_MapRender::RPG_MapRender(m_map, tiledName.c_str(), 
		TILED_SPLIT_X, m_map->getTiledSplit()/TILED_SPLIT_X+1);//生成地图渲染类
	//添加玩家人物入子画面队列,英雄NPC的Unicode=0
	RPG_NS_SpriteManager::RPG_SpriteManager::addNPC(NPC_HERO, 0, m_map, hero_X, hero_Y);
	RPG_NS_SoundManager::RPG_SoundManager::playSnd(m_map->getMapID());//播放该地图音乐
	
	//最佳视窗生成算法
	visibleWindow.x = hero_X*m_map->getTiledSize().width - GAME_WINDOW_W/2;
	visibleWindow.y = hero_Y*m_map->getTiledSize().height - GAME_WINDOW_H/2;
	visibleWindow.width = GAME_WINDOW_W;
	visibleWindow.height = GAME_WINDOW_H - 150;
	//
	if(oldmapname.compare(m_map->getMapname())){
		oldmapname = m_map->getMapname();
		m_mapName = new GUI::GUILabel("font\\simkai.ttf", 50, m_map->getMapname(), 255, 100, 20, 300, 200);
		m_mapNameIndex = 200;
	}
	//
	if(m_mapName2 != NULL){
		delete m_mapName2;
	}

	m_mapName2 = new GUI::GUILabel("font\\simkai.ttf", 15, m_map->getMapname(), 32, 255, 64, 220, 3);
	if(m_SMisExist == false){	//创建渲染引擎
		m_SMisExist = true;
		m_renderEngine = new RPG_NS_RenderEngine::RPG_RenderEngine(RPG_NS_SpriteManager::RPG_SpriteManager::getSpriteList(), 
																m_mapRender, visibleWindow);
	}
	else{
		m_renderEngine->RPG_changMap(m_mapRender, visibleWindow);
		m_renderEngine->setvisibleWindow(visibleWindow);
		m_renderEngine->adjustWindow();
	}
}

void RPG_NS_SceneManager::RPG_SceneManager::On_detectChange(){

	int iNUM;
	/*static int delay = 0;
	delay = delay<0 ? 0 : --delay; 
	if(delay > 0) return;
	delay = 150;*/
	if(!m_ableChange)//禁止切换地图
		return;
	if(m_map == NULL)
		throw "无法切换地图";
	vector<RPG_NS_Map::IO_Point> *io;	
	io = m_map->getIOPoint();	//调出地图出口信息
	RPG_NS_NPC::RPG_NPC* hero = RPG_NS_SpriteManager::RPG_SpriteManager::getNPC(m_map->getMapID(), NPC_HERO);
	for(iNUM=0; iNUM<io->size(); iNUM++){
		if(hero->getPosX() ==(*io)[iNUM].posX && hero->getPosY() ==(*io)[iNUM].posY)
			break;
		}
	if(iNUM == io->size())
		return;	//玩家没在地图出口处
	else{	//出口,切换地图
		stringstream stream;
		stream << (*io)[iNUM].next_mapID;
		string mapFileName = "map\\world"+stream.str()+".map";
		string tiledFileName = "image\\tiled\\tiled"+stream.str()+".bmp";
		//
		changeMap(mapFileName, tiledFileName, (*io)[iNUM].n_posX, (*io)[iNUM].n_posY);
	}
}

bool RPG_NS_SceneManager::RPG_SceneManager::isLoadNPC(int mapID){
	if(m_mapFlag[mapID])	//判断是否需要载入地图NPC
		return false;
	m_mapFlag[mapID] = true;
	return true;
}

void RPG_NS_SceneManager::RPG_SceneManager::RPG_GameRender(){
	
	SDL_Rect t_rect;

	RPG_NS_Map::RectType tt = m_renderEngine->getvisibleWindow();
	
	t_rect.x = tt.x;
	t_rect.y = tt.y;
	t_rect.h = tt.height;
	t_rect.w = tt.width;
	
	m_renderEngine->scrollScreen();	//卷屏
	m_renderEngine->RPG_Render();//	渲染游戏场景
	SDL_BlitSurface(m_renderEngine->getSceneBuf(), &t_rect, m_pScreen, &m_gameWindow); 
	
	RPG_NS_MenuSurface::RPG_MenuSurface::render(m_pScreen);
	//渲染对话系统
	RPG_NS_DialogSystem::RPG_DialogSystem::Update(m_pScreen);
	//
	RPG_ShowMapname();
	RPG_NS_TaskSystem::RPG_TaskSystem::render(m_pScreen);
	RPG_NS_EquipmentSystem::RPG_EquipmentSystem::render(m_pScreen);
	//SDL_Flip(m_pScreen);//将场景送屏幕显示
}

void RPG_NS_SceneManager::RPG_SceneManager::RPG_ShowMapname(){
	//
	m_smallMap->drawImage(m_pScreen, 0, 0);//画小地图
	RPG_NS_NPC::RPG_NPC* hero = RPG_NS_SpriteManager::RPG_SpriteManager::getNPC(m_map->getMapID(), NPC_HERO);
	SDL_Rect rect;
	rect.x = 737 + hero->getPosXE()*48/(m_map->getMapSize().width*m_map->getTiledSize().width);
	rect.y = 10 + hero->getPosYE()*48/(m_map->getMapSize().height*m_map->getTiledSize().height);
	rect.w = 2;
	rect.h = 2;
	SDL_FillRect(m_pScreen, &rect, SDL_MapRGB(m_pScreen->format, 255, 255, 255));
	
	if(m_mapNameIndex<=0){
		m_mapNameIndex = 0;
		if(m_mapName != NULL){
			delete m_mapName;
			m_mapName = NULL;
		}
		if(m_mapName2 != NULL)
			m_mapName2->draw(m_pScreen);
		return;
	}
	if(m_mapName != NULL){
		m_mapName->draw(m_pScreen);
		m_mapNameIndex--;
	}
}

bool RPG_NS_SceneManager::RPG_SceneManager::isInGameW(int posXE, int posYE){

	if(posXE>=m_gameWindow.x && posXE<=m_gameWindow.x+m_gameWindow.w 
		&& posYE>=m_gameWindow.y && posYE<=m_gameWindow.y+m_gameWindow.h)
		return true;
	return false;
}

RPG_NS_Map::PosType RPG_NS_SceneManager::RPG_SceneManager::convertCoordinate(int posXE, int posYE){
	//若不在游戏场景界面里返回(-1,-1)
	RPG_NS_Map::PosType t_pos;
	int t_posXE, t_posYE;
	if(isInGameW(posXE, posYE)){
		t_posXE = m_renderEngine->getvisibleWindow().x;
		t_posYE = m_renderEngine->getvisibleWindow().y;
		t_posXE += posXE;
		t_posYE += posYE;
		t_pos.posX = t_posXE/m_renderEngine->getMapRender()->getMap()->getTiledSize().width;
		t_pos.posY = t_posYE/m_renderEngine->getMapRender()->getMap()->getTiledSize().height;
	}
	else{
		t_pos.posX = -1;
		t_pos.posY = -1;
	}
	return t_pos;
}