#ifndef HEADER_RPG_SCENEMANAGER_H
#define HEADER_RPG_SCENEMANAGER_H
/*
	场景管理器（由渲染引擎和子画面管理器整合）
				结合音频管理器在地图切换时播放音效
	地图的切换:读地图，添加NPC
			   更新NPC地图指针
			   渲染场景（地图，子画面，特效）
   刷怪法则：室外地图之间切换时候清除NPC队列里所有
			 地图怪，并加载所有地图怪；在室内地图之间
			 以及室内室外地图地图切换时只有第一次进入
			 地图时加载，并且不清除NPC队列。
*/
#include <map>
#include "RPG_SpriteManager.h"
#include "RPG_Map.h"
#include "RPG_MapRender.h"
#include "RPG_RenderEngine.h"
#include "RPG_NPC.h"
#include "GUI.h"

//纹理切分方式
#define TILED_SPLIT_X 70//
#define TILED_SPLIT_Y 1//	

namespace RPG_NS_SceneManager{
	class RPG_SceneManager
	{
	private:
		static int m_currentMap;	//当前地图ID
		static RPG_NS_Map::RPG_Map* m_map;	//
		static RPG_NS_MapRender::RPG_MapRender* m_mapRender; //
		static RPG_NS_RenderEngine::RPG_RenderEngine* m_renderEngine;
		static bool m_SMisExist;	//场景管理器是否已经初始化
		static bool bChange; //是否需要切换地图
		static std::map<int, bool> m_mapFlag;	//标记是否第一次进入该地图
		static SDL_Surface* m_pScreen;	//屏幕显存
		static bool m_ableChange;	//是否允许切换地图
		static int m_climateType;	//当前天气
		//天气类型
		static const int CLIMATE_BASE = 0x100000;
		static const int SUN = CLIMATE_BASE+0x0000;//晴
		static const int RAIN = CLIMATE_BASE+0x0001;//雨天
		static const int THUNDER = CLIMATE_BASE+0x0002;//雷天
		static const int DUSK = CLIMATE_BASE+0x0004;//黄昏
		static const int DARK = CLIMATE_BASE+0x0008;//晚上
		static const int WINDY = CLIMATE_BASE+0x00016;//挂风
		//other
		static GUI::GUILabel *m_mapName;//在地图切换时显示的地图名字
		static GUI::GUILabel *m_mapName2;
		static int m_mapNameIndex; //计时因子
		static string oldmapname;
		static RPG_NS_Image::RPG_Image *m_smallMap;//小地图

	public:	
		static SDL_Rect m_gameWindow;	//游戏窗口
		
	public:
		static void RPG_SceneManager_Init(std::string mapName, std::string tiledName,
										 int hero_X, int hero_Y, SDL_Surface* pScreen);
		static void changeMap(std::string mapName, std::string tiledName, 
								int hero_X, int hero_Y);	//加载切换地图
		static void RPG_GameRender();	//渲染场景
		static void On_detectChange();	//检测是否满足切换地图要求,在消息队列中调用它
		static int getCurrentMap() {return m_currentMap;}
		static RPG_NS_Map::RPG_Map* getMap() {return m_map;}
		static RPG_NS_MapRender::RPG_MapRender* getMapRender() {return m_mapRender;}
		static RPG_NS_RenderEngine::RPG_RenderEngine* getRenderEngine() {return m_renderEngine;}
		static void setCurrentMap(int cm) { m_currentMap = cm;}	
		static bool isLoadNPC(int mapID);
		static bool isInGameW(int posXE, int posYE);	//判断鼠标是否在游戏场景界面里
		static RPG_NS_Map::PosType convertCoordinate(int posXE, int posYE);//将鼠标像素坐标转化为地图
																		   //网格坐标
		static bool canChange() {return m_ableChange;}
		static bool ableChange(bool state) {m_ableChange = state;}
		static int getClimate() {return m_climateType;}
		static void setClimate(int climate) {m_climateType = climate;} 
		static void RPG_ShowMapname();

		static SDL_Surface* getScreenbuf() {return m_pScreen;}
	};

}
#endif

