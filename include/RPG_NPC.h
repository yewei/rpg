#ifndef HEADER_RPG_NPC_H
#define HEADER_RPG_NPC_H

/*
	两个邻近瓷砖之间走动动作: [立定]->左脚,立定,右脚,立定
	该类用法:
		先构造,将位图切片,设置速度，设置两个位置坐标（要保证匹配），
		[设置瓷砖的宽跟高](构造函数会根据pMap自动设置),添加四个方向的动作队列,
		设置npc编号,设置属于哪张地图,设置朝向
*/

#include <vector>
#include <string>
#include "RPG_Image.h"
#include "RPG_Map.h"
#include <SDL\SDL.h>
using std::vector;
using std::string;

namespace RPG_NS_NPC
{
	class RPG_NPC
	{
		protected:
			int m_moveSpeed; //初始跟NPC_UPDATE_DELAY一样
			bool m_bHide; //是否显示
			bool m_bDead; //是否死亡
			int m_NPCID; //npc类型编号
			int m_NPCUnicode; //npc唯一编号
			int m_belongsToMap; //该npc属于哪张地图

		public:
			static const int NPC_UP = -1;
			static const int NPC_DOWN = 1;
			static const int NPC_LEFT = -2;
			static const int NPC_RIGHT = 2;
			static const int NPC_UPDATE_DELAY = 4; //update函数多少帧被调用一次(能决定速度)

		protected:
			RPG_NS_Image::RPG_Image* m_image; //NPC的外观位图
			RPG_NS_Map::RPG_Map* m_map;
			vector<int> m_upMotion; //存放每个方向的位图切片编号
			vector<int> m_downMotion;
			vector<int> m_leftMotion;
			vector<int> m_rightMotion;
			int m_posX, m_posY; //地图位置
			int m_oldX, m_oldY; //保存命令执行前的坐标
			int m_posXE, m_posYE; //坐标位置,地图中的像素位置(左上角)
			bool m_bBusy; //当前是否有命令在执行
			int m_moveStatus; //正在执行的命令(NPC_UP, NPC_DOWN, NPC_LEFT, NPC_DOWN),也表示朝向
			int m_currentFrame; //当前所处的位图切片
			int m_tileW, m_tileH; //瓷砖的尺寸
			//int m_mapID; //地图ID,必须独立与m_map
			int tc; //用于update计数
			int upFre; //用户update延迟

		public:
			RPG_NPC(string imageFileName, RPG_NS_Map::RPG_Map* pMap);
			RPG_NPC(string imageFileName, int mapID);
			RPG_NPC(string imageFileName);
			~RPG_NPC();
		public:
			void splitImage(int cX, int cY);
			void addDirMotion(int dir, int index); //将左脚,立定,右脚,立定,左脚,立定,右脚,立定 添加到动作队列
			void setPos(int x, int y);
			void setPosE(int x, int y);
			void setMoveSpeed(int s);
			void setTileSize(int w, int h);
			void setHide(bool isHide);
			void setDead(bool isDead);
			void setMoveStatus(int status); //设置朝向
			void setNPCID(int id);
			void setBelongsToMap(int map);
			void setNPCUnicode(int uni);
			void setMap(RPG_NS_Map::RPG_Map* map);
			int getPosX() {return m_posX;}
			int getPosY() {return m_posY;}
			int getPosXE() {return m_posXE;}
			int getPosYE() {return m_posYE;}
			int getTileW() {return m_tileW;}
			int getTileH() {return m_tileH;}
			int getMoveSpeed() {return m_moveSpeed;}
			int getMoveStatus() {return m_moveStatus;}
			int getNPCID() {return m_NPCID;}
			int getBelongsToMap() {return m_belongsToMap;}
			int getNPCUnicode() {return m_NPCUnicode;}
			int getMapID() {return m_belongsToMap;}
			bool isHide() {return m_bHide;}
			bool isDead() {return m_bDead;}
			void moveUp();
			void moveDown();
			void moveLeft();
			void moveRight();
			void move(int dir);
			bool isBusy() {return m_bBusy;}
			void update(); //两位置不一致时要更新像素坐标上的位置
			bool renderNPC(SDL_Surface* surfDest);
			RPG_NS_Image::RPG_Image* getImage() {return m_image;}
			void undoCommand();

	};
}

#endif

