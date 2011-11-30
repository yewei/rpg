#ifndef HEADER_RPG_SPRITEMANAGER_H
#define HEADER_RPG_SPRITEMANAGER_H

#include "RPG_Map.h"
#include "RPG_NPC.h"
#include "RPG_EventManager.h"

using RPG_NS_Map::RPG_Map;
using RPG_NS_NPC::RPG_NPC;
using RPG_NS_EventManager::RPG_EventHandleInterface;
using RPG_NS_EventManager::RPG_EventManager;

namespace RPG_NS_SpriteManager
{
/*
 npc?.npc的格式：
   表现层struct:定义如下

   战斗系统层struct:别处定义
*/

	typedef struct {
		int moveSpeed; //速度
		int splitX;
		int splitY; //宽度跟高度上的分块数
		int upLeft; //向上走时左脚帧所在编号
		int upRight;
		int upStop;
		int downLeft;
		int downRight;
		int downStop;
		int leftLeft;
		int leftRight;
		int leftStop;
		int rightLeft;
		int rightRight;
		int rightStop;
		int dir; //初始的朝向
	} NPC_data;

	class RPG_SpriteManager: RPG_EventHandleInterface
	{
		public:
			static const int UNICODE_BASE = 20000; //一个较大的数防止自动分配的与地图中的ID重复

		public:
			static vector<RPG_NPC*> g_collisionList; //强碰撞检测的结果队列
			static vector<RPG_NPC*> g_collisionList_non; //弱碰撞检测的结果队列
			static vector<RPG_NPC*> g_collisionList_saying; //只有两个面向相同的NPC在相邻格才算碰撞
			static void delCollisionList(int i);
			static void delCollisionList_non(int i);
			static void delCollisionList_saying(int i);
			void onRButtonDown(int mx, int my);
			void onKeyDown(SDLKey sym);
			void onRButtonOrSpace();

		private:
			static RPG_Map* m_pMap; //当前地图
			static vector<RPG_NPC*> m_spriteList; //子画面队列
			static int getNPCUnicode(); //获得一个与队列中所有NPC不一样的编号,cong 
			static void adjustment(RPG_NPC* npc); //当发生碰撞时npc位置的调整 
			static bool m_doCollision; //是否做碰撞检测
			static RPG_SpriteManager* instance;

		public:
			static void setCurrentMap(RPG_Map* pMap);
			static void setCollision(bool coll) {m_doCollision = coll;}
			static void loadNPC(RPG_Map* pMap); //载入地图中的NPC
			static void killNPC(int mapID); //删除地图编号为mapID中的所有NPC
			static void killNPCByIDs(int mapID, int NPCUnicode);
			static void killAllNPC(); //删除除了自定义子画面外的所有子画面
			static int addNPC(int NPCID, int NPCUnicode, int mapID, int x, int y);
			static int addNPC(int NPCID, int NPCUnicode, RPG_Map* pMap, int x, int y);
			static int addNPC(int NPCID, int NPCUnicode, RPG_Map* pMap, int mapID, int x, int y); //添加属于mapID的NPC,返回NPCUnicode
			static void clearList(); //删除所有子画面，清空队列
			static vector<RPG_NPC*>* getSpriteList() {return &m_spriteList;}
			static RPG_NPC* getNPC(int mapID, int NPCUnicode);
			static void setMap(int mapID, RPG_Map* pMap); //将子画面列表中mapID的子画面地图指针指向pMap
			
		public:
			static void init();
			static void updateSprites(); //更新当前地图中的子画面状态
			static void detectCollision(); //碰撞检测
			static void killDeadSprite(); //删除死亡的子画面,一般战斗后被调用
	};
}



#endif