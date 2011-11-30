#ifndef HEADER_FIGHTSYS_H
#define HEADER_FIGHTSYS_H
#include "game.h"
#include <vector>
#include "RPG_NPC.h"
#include "RPG_Map.h"
#include "SDL\SDL.h"
#include "GUI.h"
#include "RPG_EventManager.h"

#define MAX_GET_EQUIPMENT 10//NPC最多可掉装备数
#define MAX_NPC_MAGIC 5//NPC最多可使用魔法数 
#define MAX_ATTIME 1000 //AT时间终点
//#define MAP_WIDTH 10 //战斗地图大小
//#define MAP_HEIGHT 10 


using std::vector;

namespace RPG_NS_FightSys{//战斗系统

	
	typedef struct{
		int mgxs; //魔攻系数
		int zdl;//致毒率
		int zdsh;//致毒伤害
		int zsml;//致睡眠率
		int fml;//封魔率
		int speeddown;//减速
		int stringdown;//减攻
		int defdown;//减防
	}Magic_attack;//魔法攻击属性
	
	typedef struct{
		int hx;//回血量
		int speedup;//加速
		int stringup;//加攻
		int defup;//加防
	}Magic_def;//魔法防御属性
		
	typedef struct{
		char name[MAX_NAME];//魔法名称
		int type;//魔法类型作用(0:敌方单体 1:敌方群体 2:我方单体 3:我方群体)
		int length;//攻击距离
		int slfw;//群体性魔法作用范围（曼哈顿距离）
		int manacost;//魔法消耗
		Magic_attack attack;
		Magic_def defend;
	}Magic;//魔法

	//
	typedef struct{
		int wg; //物攻buf + -
		int wgCounter;//物攻buf剩余回合数
		int wf;//物防
		int wfCounter;
		int dsh;//毒伤害
		int dCounter;
		int speed;//速度buf
		int speedCounter;
		int sleepCounter;//睡眠剩余回合数
		int fmCounter;//封魔剩余回合数
	}Buf;//附加状态

	typedef struct{
		HeroStat state;
		int fid;//战斗id
		int attacklength;//物攻范围
		int movelength;//移动范围
		Buf buf;//附加状态
	}Hero;

	typedef struct{
		int fid;//战斗ID
		int hp;
		int wg;
		int wf;
		int mg;
		int mf;
		int speed;
		int attacklength;//物攻范围
		int movelength;//移动范围
		Buf buf;//附加状态
		char magic[MAX_NPC_MAGIC][MAX_NAME];//npc携带的魔法
		//
		int exp;//杀掉可得经验数
		int money;//掉钱数
		char equipment[MAX_GET_EQUIPMENT][MAX_NAME];//可掉装备
		double rate[MAX_GET_EQUIPMENT];//相应装备可掉概率
	}NPC;

	class RPG_FightSystem : public GUI::GUIObject, RPG_NS_EventManager::RPG_EventHandleInterface
	{
	public:
		static const int ATTACK = 0;
		static const int MOVE = 0;
		static const int MAGIC = 0;
		static const int MEDICIN = 0;

	private:
		vector<Magic> magic_list;//
		vector<Hero> hero_list;
		vector<NPC> npc_list;//战斗npc队列
		vector<RPG_NS_NPC::RPG_NPC> npclist;//npc显示队列
		vector<int> action_list;//行动队列
		int m_numOfP; //进入战斗系统中的怪物数量
		RPG_NS_Map::RPG_Map *m_map;
		SDL_Rect m_area; //战斗区域
		SDL_Surface* m_pScreen;
		GUI::GUIButton* m_btnMove;//玩家移动按钮
		GUI::GUIButton* m_btnAttack;//物理攻击
		GUI::GUIButton* m_btnMagic;//使用魔法
		GUI::GUIButton* m_btnMedicine;//吃药
		//
		bool fight_over;//战斗系统是否开启
		int action_sel;//玩家选择行动类型 0:攻击 1:移动 2：魔法 3:药
		int current_npc;//当前活动的npc,英雄编号为0~2，其他从3开始编号
		vector<int> npcATtime; //所有人物AT时间,总时间长度为1000
		bool waitMove;//等待完成move操作
		//
		void Internal_init();//内部变量初始化
		int ATreach();//返回到达时刻单位的战斗id
		void playerCtl(); //玩家控制
		void NPC_Render();	
		void render();
		void onMove(); //单击控制界面的“移动”事件
		
		//
		static void move_Helper(GUI::GUIObject* object){
			((RPG_FightSystem*)object)->onMove();
		}
 	public:
		static void RPG_runFightSys(int npcID, int npcUni);//这里的npcID并非作战系统里的npc ID，需要
														//通过转换文件 npcIDtrans.dat 进行转化
		void loadHero();
		void loadNPC(int npcID, int npcUni);//载入npc
		void loadMagic();
		void fightCycle();//进入战斗系统主循环
		void initMap();

		RPG_FightSystem(int npcID, int npcUni);
		~RPG_FightSystem(void);
		virtual void onLButtonDown(int mx, int my);
		virtual void onRButtonDown(int mx, int my);
	};
}
#endif
