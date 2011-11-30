/***************************************/
/*********** .map文件格式约定 **********/
/*
	mapID \n
	mapsize(width, height) \n
	tiledsize(width, height) \n
	map_front_layer(width*height 矩阵)
	map_back_layer(width*height 矩阵)
	map_logic_layer(width*height 矩阵)
	NPC 个数
	NPC 信息
		其中NPC信息格式约定：
		ID-UnicodeID-固定位置/随机位置[<如果随机> 随机位置个数 ...]-是否随机出现[<是> 概率]
					[<如果固定> 固定位置]
	地图进出口 个数
	地图进出口信息
		其中地图进出口信息约定：
		posX posY(本地图进出口坐标) nextMapID(下张地图的ID) n_posX n_posY(下张地图的入口坐标)
	地图纹理个数
*/
/**************************************/
#include "../include/RPG_map.h"
#include <cstdio>
#include <memory.h>
#include <time.h>

unsigned int RPG_NS_Map::RPG_Map::convertToIndex(int posX, int posY)	//坐标位置到地图块索引号的转化
{
	return posY*m_mapSize.width+posX;
}

RPG_NS_Map::PosType RPG_NS_Map::RPG_Map::convertToPos(unsigned int index) 	//地图块索引号到坐标位置的转化
{
	int posX,posY;
	PosType t;
	posX = index % m_mapSize.width;
	posY = index / m_mapSize.width;
	t.posX = posX;
	t.posY = posY;
	return t;
}

bool RPG_NS_Map::RPG_Map::canMov(int posX, int posY)	//判断地图是否可通行
{
	if(posX>=m_mapSize.width || posY>=m_mapSize.height ||
		posX<0 || posY<0)
		return false;
	return !(*(m_mapDataLogic+posY*m_mapSize.width+posX));
}

bool RPG_NS_Map::RPG_Map::canMov(unsigned int index)
{
	PosType t;
	t = convertToPos(index);
	return !(*(m_mapDataLogic+t.posY*m_mapSize.width+t.posX));
}

RPG_NS_Map::RPG_Map::RPG_Map(string mapFileName, bool isLoadNPC)//地图类构造
{
	FILE *mapFile;
	int numOfNPC;
	char mapName[100];
	Read_NPC t_readNpc;
	mapFile = fopen(mapFileName.c_str(), "r");
	if(mapFile == NULL)
		throw "找不到地图文件："+mapFileName;
	fscanf(mapFile, "%d", &m_mapID);	//按照.map文件约定读入
	fscanf(mapFile, "%s", mapName);	//读入地图名称
	m_mapName = mapName;
	fscanf(mapFile, "%d %d", &m_mapSize.width, &m_mapSize.height);
	fscanf(mapFile, "%d %d", &m_tiledSize.width, &m_tiledSize.height);
	//分配地图内存空间
	m_mapDataFront = (int*)malloc(m_mapSize.width*m_mapSize.height*sizeof(int));
	m_mapDataBack = (int*)malloc(m_mapSize.width*m_mapSize.height*sizeof(int));
	m_mapDataLogic = (int*)malloc(m_mapSize.width*m_mapSize.height*sizeof(int));
	if(m_mapDataFront==NULL || m_mapDataBack==NULL || m_mapDataLogic==NULL)
		throw "无法分配地图内存";

	//读入矩阵
	for(int iLoop=0; iLoop<m_mapSize.height; iLoop++)
		for(int jLoop=0; jLoop<m_mapSize.width; jLoop++){
			fscanf(mapFile, "%d", m_mapDataFront+iLoop*m_mapSize.width+jLoop);
		}
	for(int iLoop=0; iLoop<m_mapSize.height; iLoop++)
		for(int jLoop=0; jLoop<m_mapSize.width; jLoop++){
			fscanf(mapFile, "%d", m_mapDataBack+iLoop*m_mapSize.width+jLoop);
		}
	for(int iLoop=0; iLoop<m_mapSize.height; iLoop++)
		for(int jLoop=0; jLoop<m_mapSize.width; jLoop++){
			fscanf(mapFile, "%d", m_mapDataLogic+iLoop*m_mapSize.width+jLoop);
		}

	//读入NPC信息
	
	fscanf(mapFile, "%d", &numOfNPC);
	for(int npcIndex=0; npcIndex<numOfNPC; npcIndex++){
		fscanf(mapFile, "%d", &t_readNpc.npcID);
		fscanf(mapFile, "%d", &t_readNpc.npcUnicode);
		fscanf(mapFile, "%d", &t_readNpc.isRandom);
		if(t_readNpc.isRandom){	//随机位置
			fscanf(mapFile, "%d", &t_readNpc.numOfRND);
			t_readNpc.posRND = (PosType *)malloc(t_readNpc.numOfRND*sizeof(PosType));
			for(int iRND=0; iRND<t_readNpc.numOfRND; iRND++)
				fscanf(mapFile, "%d %d", &t_readNpc.posRND[iRND].posX,
										&t_readNpc.posRND[iRND].posY);
		}
		else{	//确定位置
			t_readNpc.posRND = (PosType *)malloc(sizeof(PosType));
			fscanf(mapFile, "%d %d", &t_readNpc.posRND[0].posX,
										&t_readNpc.posRND[0].posY);
		}
		fscanf(mapFile, "%d", &t_readNpc.isRndShow);
		if(t_readNpc.isRndShow){	//随机出现
			fscanf(mapFile, "%d", &t_readNpc.Posibility);
		}
		if(isLoadNPC)
			npcInit(t_readNpc);	//填充NPC队列
		
	}
	//
	fscanf(mapFile, "%d", &m_numOfIO);
	for(int iNum=0; iNum<m_numOfIO; iNum++){
		IO_Point t;
		fscanf(mapFile, "%d %d %d %d %d", &t.posX, &t.posY, &t.next_mapID, &t.n_posX, &t.n_posY);
		m_inoutPointList.push_back(t);
	}
	fscanf(mapFile, "%d", &m_tiledSplit);	//读取地图纹理个数
	fclose(mapFile);
}

RPG_NS_Map::RPG_Map::~RPG_Map()//析构
{
	//释放地图空间
	free(m_mapDataFront);
	free(m_mapDataBack);
	free(m_mapDataLogic);
}

void RPG_NS_Map::RPG_Map::npcInit(Read_NPC t_npc)
{
	int rnd;
	NPC npc;
	rnd = rand()%100;	//产生随机数
	if(t_npc.isRndShow){
		if(rnd>t_npc.Posibility)
			return;
	}
	npc.NPC_ID = t_npc.npcID;
	npc.NPC_Unicode = t_npc.npcUnicode;
	if(t_npc.isRandom){	//随机出现
		rnd = rand()%t_npc.numOfRND;
		npc.pos = t_npc.posRND[rnd];
	}
	else{
		npc.pos = t_npc.posRND[0];
	}
	m_npcList.push_back(npc);		//添加入NPC队列
}

RPG_NS_Map::SizeType RPG_NS_Map::RPG_Map::getMapSize_E(){

	SizeType t_size;
	t_size.width = m_mapSize.width * m_tiledSize.width;
	t_size.height = m_mapSize.height * m_tiledSize.height;
	return t_size;
}