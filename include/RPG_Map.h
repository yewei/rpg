#ifndef HEADER_GAME_MAPH
#define HEADER_GAME_MAPH
#include<vector>
#include<string>
using std::vector;
using std::string;
namespace RPG_NS_Map{

	typedef struct {
		int posX;
		int posY;
	}PosType;

	typedef struct {
		int width;
		int height; 
	}SizeType;

	typedef struct {
		int x;
		int y;
		int width;
		int height;
	}RectType;

	typedef struct {
		int NPC_Unicode;	//NPC地图全局唯一标识
		int NPC_ID;	//NPC ID号
		PosType pos;
	}NPC;

	typedef struct {
		int npcUnicode;
		int npcID;
		int isRandom;
		int numOfRND;
		PosType *posRND;
		int isRndShow;
		int Posibility;
	}Read_NPC;	//NPC读入信息格式

	typedef struct {	//进出口信息
		int posX;
		int posY;
		int next_mapID;
		int n_posX;
		int n_posY;
	}IO_Point;
	
	class RPG_Map	//地图类	
	{
	protected:
		int m_mapID;					//地图编号
		SizeType m_tiledSize;			//纹理大小
		SizeType m_mapSize;				//地图大小	
		int* m_mapDataFront;	//地图表现层(前背景)
		int* m_mapDataBack;	//地图表现层(后背景)
		int* m_mapDataLogic;	//地图逻辑层（碰撞检测层）
		vector<NPC> m_npcList;			//NPC
		int m_numOfIO;	//进出口数
		vector<IO_Point> m_inoutPointList;				//进出口点队列
		int m_tiledSplit;	//地图纹理个数
		string m_mapName; //地图名称

	public:
		RPG_Map(string mapFileName, bool isLoadNPC);
		~RPG_Map();
		void setMapID(int id) {m_mapID = id;}
		int getMapID() {return m_mapID;}
		int* getMapDataFront() {return m_mapDataFront;}	
		int* getMapDataBack() {return m_mapDataBack;}
		int* getMapDataLogic() {return m_mapDataLogic;}
		vector<IO_Point>* getIOPoint() {return &m_inoutPointList;}
		vector<NPC> getNpcList() {return m_npcList;}
		SizeType getMapSize() {return m_mapSize;}
		SizeType getTiledSize() {return m_tiledSize;}
		bool canMov(int posx, int posy);	//判断地图是否可通行
		bool canMov(unsigned int index);	//
		unsigned int convertToIndex(int posX, int posY);	//坐标位置到地图块索引号的转化
		PosType convertToPos(unsigned int index);	//地图块索引号到坐标位置的转化
		void npcInit(Read_NPC npc);	//地图初始化
		SizeType getMapSize_E(); //得到地图像素尺寸
		int getTiledSplit() {return m_tiledSplit;}
		string getMapname() {return m_mapName;}
	};
}
#endif