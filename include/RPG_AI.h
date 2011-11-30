/**********
*******data/ai/npcmove/all.npcmove
定义了NPC在地图中的活动情况
例：
1 ；npcId种类
5 ; 运动势力范围
4 ；4种运动
0 ; 0表示在势力范围内随机移动，在势力范围内随机一个地点
1 10; 1表示能追赶，弱碰撞时一定采取该行为， 10表示最大的追赶范围
2 ; 横向运动，走到左边的势力范围，回到原点，再向右再回来
3 ; 纵向运动，。。。
-1 ; 文件结束

***********/



#ifndef HEADER_RPG_AI_H
#define HEADER_RPG_AI_H

#include "RPG_NPC.h"
#include "RPG_SpriteManager.h"
#include <vector>

using std::vector;
using RPG_NS_NPC::RPG_NPC;
using RPG_NS_SpriteManager::RPG_SpriteManager;

namespace RPG_NS_AI
{
	typedef struct{
		int npcId;
		int length; //该NPC能运动的势力范围
		int actionLength; //有多少种运动类型
		int actionType[4]; //运动的类型，不包括追赶
		int huntLength; //如果有追赶行为，那么追赶的势力范围，否则为-1
	}NPCAction; //描述某个NPCID的行为

	typedef struct{
		int mapId;
		int npcId; 
		int npcUnicode; //该NPC的地图中唯一编码
		RPG_NPC* npc; //该NPC的指针
		int orgX, orgY; //该NPC第一次出现时的位置
		int nowX, nowY; //该NPC当前位置
		vector<int> actionList; //动作队列
		bool inHunt; //当前的行为是否是追击行为
		bool goHome; //当前的行为是回原点的行为
	}AIItem;

	class RPG_AI
	{
		private:
			static vector<NPCAction> m_npcActionList; //所有NPC的行为
			static vector<AIItem> m_aiItem; //维护着当前npc的具体行为, 需要每帧更新

			static void updateList(); //更新m_aiItem队列，与子画面队列保持一致
			static NPCAction getActionInfo(int npcId); //根据npcId得到该类型的行为

		public:
			static void RPG_AI_Init(); //初始化，载入行为文件等
			static void AIMove(); //每帧调用，实现ＡＩ
	};
}


#endif HEADER_RPG_AI_H