#ifndef HEADER_RPG_LEARNMAGIC
#define HEADER_RPG_LAERNMAGIC

/******************
******data/learnMagic/*.learn 某个npc可以教授的魔法
文件名是mapID*U*.learn
文件格式:
魔法名称
魔法描述
价格
魂1 魂2 魂3 魂4 ;学习该魔法英雄需要多少点数
END
******************/

#include <vector>
#include <SDL\SDL.h>
#include "game.h"
#include "GUI.h"
#include "RPG_Image.h"
using std::vector;

namespace RPG_NS_LearnMagic
{
	typedef struct{
		char magicName[MAX_NAME]; 
		char magicDesc[256];
		int cost;
		int soul[4];
	}MagicItem;

	class RPG_LearnMagic : public GUI::GUIObject
	{
		public:
			static const int LEARN_NULL = 0;
			static const int LEARN_LIVE = 1;
			static const int LEARN_DEAD = 2;

			static const int HEROBASEX = 430;
			static const int HEROBASEY = 115;
			static const int LEARNBASEX = 150;
			static const int LEARNBASEY = 100;

		private:
			static RPG_LearnMagic* instance;
			static int status;
			vector<MagicItem> magicList; //该NPC所卖魔法列表
			int m_currentHero;

			MagicItem getMagicItem(string magiccName);

		private: //GUI
			RPG_NS_Image::RPG_Image* m_back;
			GUI::GUIMessageBox* m_message;
			////////////////英雄相关界面/////////////////
			GUI::GUIButton* m_button_left; //向左按钮
			GUI::GUIButton* m_button_right; //向右按钮	
			GUI::GUIButton* m_button_add1; //加魂1
			GUI::GUIButton* m_button_add2; //加魂2
			GUI::GUIButton* m_button_add3; //加魂3
			GUI::GUIButton* m_button_add4; //加魂4

			GUI::GUILabel* m_label_name; //英雄姓名
			GUI::GUILabel* m_label_zsh; //"总四魂:"
			GUI::GUILabel* m_label_totSoul; //描述当前英雄总的四魂
			GUI::GUILabel* m_label_pointSoul; //剩余的点数
			GUI::GUILabel* m_label_soul[4]; //剩余魂

			GUI::GUIListBox* m_listbox_heroMagic; //英雄的已有魔法

			////////////////卖方相关界面//////////////////
			GUI::GUIButton* m_button_buy; //购买按钮
			GUI::GUIButton* m_button_exit; //退出按钮

			GUI::GUIListBox* m_listbox_learnMagic; //卖的魔法

			GUI::GUILabel* m_label_desc;
			GUI::GUILabel* m_label_cost;
			GUI::GUILabel* m_label_needSoul;
			GUI::GUILabel* m_label_money; //玩家金钱


			/////////////////按钮事件////////////////////
			void onBuy();
			void onExit();
			void onLeft();
			void onRight();
			void onAdd1();
			void onAdd2();
			void onAdd3();
			void onAdd4();
			static void onBuyHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onBuy();
			}
			static void onExitHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onExit();
			}
			static void onLeftHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onLeft();
			}
			static void onRightHelper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onRight();
			}
			static void onAdd1Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd1();
			}
			static void onAdd2Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd2();
			}
			static void onAdd3Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd3();
			}
			static void onAdd4Helper(GUI::GUIObject* object){
				((RPG_LearnMagic*)object)->onAdd4();
			}


			////////////////////////////////////////////
			void createHeroInterface(int index); //重构英雄相关的GUI

		public:
			RPG_LearnMagic(int mapId, int unicode); //标识哪一个npc
			~RPG_LearnMagic();
			static void render(SDL_Surface* destSurf);
			
	};
}

#endif