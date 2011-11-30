#ifndef HEADER_RPG_RENDERENGINE_H
#define HEADER_RPG_RENDERENGINE_H
/*
	地图渲染引擎：
	渲染地图背景子层->NPC子层->地图前景层->特效天气层
	支持滚屏

*/

#include "RPG_NPC.h"
#include "RPG_MapRender.h"
#include "SDL\SDL.h"
#include "GUI.h"

namespace RPG_NS_RenderEngine{
	class RPG_RenderEngine
	{
	private:
		SDL_Surface* m_scene;		//场景缓冲区
		vector<RPG_NS_NPC::RPG_NPC*> *m_renderList;	//NPC渲染队列
		RPG_NS_MapRender::RPG_MapRender *m_mapRender;	//地图渲染类
		RPG_NS_Map::RectType m_visibleWindow;	//视窗
		RPG_NS_Map::RectType m_largeView;		//大视窗(所有在此窗口内的NPC都要被渲染)
		RPG_NS_Map::RectType m_smallView;		//小视窗(人物在此窗口内移动不会触发滚屏)
		int m_currentMap; //目前地图编号
		int m_special;	//特效（天气）编号

	public:
		RPG_RenderEngine(vector<RPG_NS_NPC::RPG_NPC*> *renderList, RPG_NS_MapRender::RPG_MapRender *mapRender,
							RPG_NS_Map::RectType visibleWindow);
		~RPG_RenderEngine(void);
	public:
		void RPG_Render();
		void RPG_Sprite_Render();
		void setMapRender(RPG_NS_MapRender::RPG_MapRender *mapRender) {m_mapRender = mapRender;}
		void renderEngine_Init();	//初始化渲染引擎
		RPG_NS_Map::RectType getvisibleWindow() {return m_visibleWindow;}
		RPG_NS_Map::RectType getlargeView() {return m_largeView;}
		RPG_NS_Map::RectType getsmallView() {return m_smallView;}
		RPG_NS_MapRender::RPG_MapRender* getMapRender() {return m_mapRender;}
		SDL_Surface* getSceneBuf() {return m_scene;}
		void setvisibleWindow(RPG_NS_Map::RectType rect) { m_visibleWindow = rect;}
		void setlargeView(RPG_NS_Map::RectType rect) { m_largeView = rect;}
		void setsmallView(RPG_NS_Map::RectType rect) { m_smallView = rect;}
		void setCurrentMap(int id) { m_currentMap = id;}
		void adjustWindow();	//根据视口调整视窗
		void scrollScreen();	//根据人物位置滚卷所有窗口
		void RPG_SpecialScene();	//特效场景处理
		void RPG_changMap(RPG_NS_MapRender::RPG_MapRender *mapRender, RPG_NS_Map::RectType vw);	//切换地图
	};
}

#endif
