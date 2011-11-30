#ifndef HEADER_GAME
#define HEADER_GAME


/******
****data\item\all.item所有物品的描述
typede struct{
	char material[MAX_NAME]; 名称
	int cost; 价格
	char desc[256]; 描述
	....
}Item;
*******/

#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

#define MAX_NAME 20


/**********************物品的存储信息************************/
#define RADIO_BASE 10000 //数字为10000以后的是百分比的单位
typedef struct{
	int zdxl; //最大血量
	int zdml; //最大魔量
	
	int wg; //物攻
	int wf; //物防
	int mg; //魔攻
	int mf; //魔防
	int sudu; //速度

	int bjl; //暴击率
	int zdl; //致毒率
	int zdsh; //致毒伤害
	int zsml; //致睡眠率
	int yjbsl; //一击必杀率
	int fjl; //封技率
	int fml; //封魔率

	int fdl; //防毒率
	int fsml; //防睡眠率
	int fyjbsl; //防一击必杀率
	int ffjl; //防封技率
	int ffml; //防封魔率
	int jdsh; //减毒伤害
	int hhhx; //回合回血
	int hhhm; //回合回魔
	int hhhcp; //回合回CP
} EquipmentStat; //装备相关的人属性

typedef struct{
	int canUse; //在装备系统中能否使用
	int addBlood; //加血
	int addMana; //加魔
	int addCp; //加CP
} DrugStat; //药的信息

typedef struct{
	char material[MAX_NAME];//物品名称
	int cost;//物品的价格
	char desc[256];//对物品的描述
	int type; //0表示武器,2表示首饰(0,2都是装备号),1表示药品类,3表示剧情物品
	int eqpID; //只有为装备时才有效,表示具体是什么类别
	EquipmentStat eqs; //装备的详细信息
	DrugStat dgs; //药品类的详细信息
} Item; //物品详细信息(最终存储结构)

/***********************************************************/


/*********************英雄的状态与包裹************************/
	
typedef struct{
	char material[MAX_NAME]; //Hero拥有的物品名称
	int num; //该物品的数量
} Mate; //人物身上的物品

typedef struct{
	char heroName[MAX_NAME];//英雄的名字
	int level;//英雄的等级
	int experience; //当前经验
	int nextExperience; //下一级的经验
	int blood;//当前的血量
	int mana;//当前的魔量
	int cp;//当前的CP
	int soul[4]; //剩余四魂的值
	int totSoul[4]; //总共四魂的值
	int pointSoul; //多少点数没有加
	vector<string> magicList; //英雄身上的魔法列表
	EquipmentStat attribute;//英雄的详细信息
} HeroStat; //英雄的状态,包括血、魔、攻防、...
typedef struct{
	HeroStat heros[3];//最多三个英雄
	vector<Mate> items[3]; //英雄身上的装备
	int heroCount;
} HeroSet;
extern HeroSet heroSet;//英雄集合

typedef struct{
	int finance; //玩家的钱数
	vector<Mate> weapon; //武器装备
	vector<Mate> drug; //药品
	vector<Mate> jewelry; //首饰
	vector<Mate> story; //剧情物品
}Package; //包裹中的东西
extern Package heroPackage; //在game.cpp中定义的全局包裹

/***********************************************************/

/***********************游戏的状态信息************************/

extern bool gamePause; //游戏暂停标记
extern bool gameOver; //游戏停止标记
extern int gameChapter; //当前的章节,从1开始编号
extern map<string, int> sema; //剧情信号量集
void story_tot(); //游戏剧情机


/*******************全局的辅助函数************************/
string newLine(string str, int n); //将str组成每行最多n个汉字的新字符串
void addToPackage(int witch, string material); //添加一个名称为material的物品,0是weapon包,1是drug...
void delFromPackage(int witch, string material); //删除一个名称为material的物品
void addToHero(int nameid, string material); //添加装备material到nameid英雄身上
void delFromHero(int nameid, string material); //删除英雄nameid身上的material
bool hasGood(string material); //测试包裹中是否有该物品

#endif


/*********************************************************/

