#ifndef HEADER_DLGSYSTEM_H
#define HEADER_DLGSYSTEM_H

#include <string>
#include "GUI.h"
#include "RPG_Image.h"
#include <SDL\SDL.h>
#include "RPG_EventManager.h"

/*	对话系统
	每张地图生成一个*.say的对话文件：
	....
	-255 NULL NULL
	NpcUnicodeID(int) npcName(string) NULL
	flag(说话者标记->0:英雄1；-1:英雄2；-2：英雄3；1：NPC) bmpFileName(头像文件路径) saystring(说话内容)
	-255 NULL NULL
*/
using RPG_NS_Image::RPG_Image;

namespace RPG_NS_DialogSystem{
	class RPG_DialogSystem : public RPG_NS_EventManager::RPG_EventHandleInterface
	{
	public:
		static const int DLG_NULL = 0; //表示当前没有开启对话系统
		static const int DLG_LIVE = 1; //表示当前开启对话系统
		static const int DLG_DEAD = 2; //表示关闭对话系统,需要删除实例
		//
		static const int BK_Y = 450; //对话框背景
		static const int PICTURE_X = 40; //头像显示坐标
		static const int PICTURE_Y = 470;
		static const int SAYNAME_X = 200; //说话者名字显示坐标
		static const int SAYNAME_Y = 470; 
		static const int SAYCONTENT_X = 200; //对话内容显示坐标
		static const int SAYCONTENT_Y = 500; 
	private:
		static int status; //当前的状态(DLG_NULL, DLG_LIVE, DLG_DEAD)
		static RPG_DialogSystem* instance; //实例
		//
		int m_npcID;	//npc唯一编号
		GUI::GUILabel *m_npcName; //npc名称
		std::string m_heroName[3]; //三英雄名字
		RPG_Image *m_picture;//头像
		RPG_Image *m_back;//对话显示背景图层
		GUI::GUILabel *m_currentSay; //当前说话内容
		std::string m_content;//整句说话内容
		GUI::GUILabel *m_nameOfCurrentSay;//当前叙述者名字
		//
		bool bInDialog;//是否处于对话过程中
		FILE *m_sayFile;//对话文件
		static bool saySentence;//是否在说某句话的过程中
		bool m_flag;
		static std::string saypath;
		static std::string picturePath;
		static void update(SDL_Surface* destSurf);//aid function:对话过程中的更新
		static std::string getHZbyIndex(std::string str, int index);//通过索引值取字符汉字

	public:
		void setHeroName(int heroID, std::string name){m_heroName[heroID] = name;}
		static void createDialog(int mapID, int npcUnicodeID);
		static void Update(SDL_Surface* destSurf);//每个游戏循环中调用
		static string intostring(int n);
		//
		//virtual void onRButtonDown(int mx, int my);//
		virtual void onKeyDown(SDLKey sym);
		virtual void onLButtonDown(int mx, int my);

	public:
		RPG_DialogSystem(int mapID, int npcUnicodeID, std::string *heroName);
		~RPG_DialogSystem(void);
	};
}
#endif