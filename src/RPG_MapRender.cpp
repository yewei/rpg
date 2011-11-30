/**********************************/
/***        地图渲染类：     ******/
/* 将地图背景和前景纹理分别贴
   到内存的相关区域				 **/
/**********************************/
#include "../include/RPG_mapRender.h"
#include "../include/RPG_Config.h"
void RPG_NS_MapRender::RPG_MapRender::mapRender_Init(string tiledFile,
													  int x_tiled ,int y_tiled){	//地图渲染器初始化
	RPG_NS_Map::SizeType mapSize;
	RPG_NS_Map::SizeType tiledSize;
	mapSize = m_rpgMap->getMapSize();
	tiledSize = m_rpgMap->getTiledSize();
	m_tiledSet = new RPG_NS_Image::RPG_Image(tiledFile); //加载纹理
	if(m_tiledSet == NULL)
		throw "加载纹理失败";
	m_tiledSet->splitImage(x_tiled, y_tiled); //纹理文件切分为x_tiled*y_tiled个小纹理
	m_numOfTiled = x_tiled * y_tiled;

	m_backMapBuf = SDL_CreateRGBSurface(SDL_SWSURFACE, mapSize.width*tiledSize.width, mapSize.height*tiledSize.height, 
						 				 32, 0, 0, 0, 0);
	if(m_backMapBuf == NULL)
		throw "无法分配地图背景内存";
	m_frontMapBuf = SDL_CreateRGBSurface(SDL_SWSURFACE, mapSize.width*tiledSize.width, mapSize.height*tiledSize.height, 
										 32, 0, 0, 0, 0);
	if(m_frontMapBuf == NULL)
		throw "无法分配地图前景内存";

	SDL_FillRect(m_frontMapBuf, NULL, SDL_MapRGB(m_frontMapBuf->format, 0xff, 0, 0xff)); //填充透明色
	SDL_SetColorKey(m_frontMapBuf, SDL_SRCCOLORKEY, SDL_MapRGB(m_frontMapBuf->format, 0xff, 0, 0xff)); //设置透明色


	//渲染
	frontMap_Render();
	backMap_Render();
	
}
void RPG_NS_MapRender::RPG_MapRender::frontMap_Render()//前景渲染
{
	int xTiled,yTiled;
	int* mapDataFront;
	RPG_NS_Map::SizeType tiledSize,mapSize;
	mapDataFront = m_rpgMap->getMapDataFront();
	tiledSize = m_rpgMap->getTiledSize();
	mapSize = m_rpgMap->getMapSize();
	for(yTiled=0; yTiled<mapSize.height; yTiled++)
		for(xTiled=0; xTiled<mapSize.width; xTiled++){
			if(*(mapDataFront+yTiled*mapSize.width+xTiled) != -1)
				m_tiledSet->drawSpliter(m_frontMapBuf, tiledSize.width*xTiled,
				tiledSize.height*yTiled, *(mapDataFront+yTiled*mapSize.width+xTiled));
		}
}

void RPG_NS_MapRender::RPG_MapRender::backMap_Render()//背景渲染
{
	int xTiled,yTiled;
	int* mapDataBack;
	RPG_NS_Map::SizeType tiledSize,mapSize;
	mapDataBack = m_rpgMap->getMapDataBack();
	tiledSize = m_rpgMap->getTiledSize();
	mapSize = m_rpgMap->getMapSize();
	for(yTiled=0; yTiled<mapSize.height; yTiled++)
		for(xTiled=0; xTiled<mapSize.width; xTiled++){
			if(*(mapDataBack+yTiled*mapSize.width+xTiled) != -1)
			m_tiledSet->drawSpliter(m_backMapBuf, tiledSize.width*xTiled,
				tiledSize.width*yTiled, *(mapDataBack+yTiled*mapSize.width+xTiled));
		}
}

RPG_NS_MapRender::RPG_MapRender::RPG_MapRender(class RPG_NS_Map::RPG_Map* rpgMap,
											   string tiledFile, int x_tiled, int y_tiled){//构造函数
	m_rpgMap = rpgMap;
	mapRender_Init(tiledFile, x_tiled, y_tiled);	//初始化地图渲染器
}

RPG_NS_MapRender::RPG_MapRender::~RPG_MapRender()
{
	SDL_FreeSurface(m_backMapBuf);
	SDL_FreeSurface(m_frontMapBuf);
	delete m_tiledSet;
	delete m_rpgMap;
}