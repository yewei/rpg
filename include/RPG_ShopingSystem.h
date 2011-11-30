#ifndef HEADER_RPG_SHOPINGSYSTEM
#define HEADER_RPG_SHOPINGSYSTEM

/*******
****data\shop\*.shop某个商人的买卖物品
文件格式:
武器装备
~
药品
~
首饰
END
******
每个卖家一个文件，构造函数传mapID跟unicodeID来产生文件名。
例：
mapID:5
UnicodeID:1
文件名:mapID5U1.shop
********/

#include <SDL\SDL.h>
#include <string>
#include "GUI.h"
#include "game.h"
#include "RPG_Config.h"
#include "RPG_Image.h"
using std::string;

namespace RPG_NS_ShopingSystem
{
	class RPG_ShopingSystem: public GUI::GUIObject
	{
		public:
			static const int SHOP_NULL = 0; //表示当前没有开启买卖系统
			static const int SHOP_LIVE = 1; //表示当前开启买卖系统
			static const int SHOP_DEAD = 2; //表示关闭买卖系统,需要删除实例
			static const int SHOP_STARTX = GAME_WINDOW_W/2-100;
			static const int SHOP_STARTY = GAME_WINDOW_H/2-200;
			static const int SHOP_TOP = 0; //表示显示顶层按钮
			static const int SHOP_BUY = 1; //表示显示买东西界面
			static const int SHOP_SALE = 2; //表示显示卖东西界面
			static const int SHOP_SAY = 3; //表示显示说话界面

		private:
			static int status; //当前的状态(SHOP_NULL, SHOP_LIVE, SHOP_DEAD)
			static RPG_ShopingSystem* instance; //实例
			vector<Item> m_itemList; //所有物品的列表,查找信息用
			vector<string> m_weaponList; //所卖武器装备
			vector<string> m_drugList; //所卖药品
			vector<string> m_jewelryList; //所卖首饰
			int m_interface; //当前显示的界面
			int m_wdj; //买卖时当前是"武器","药品"还是"首饰" 0 1 2
			int m_mapID;
			int m_npcUnicode;
			bool m_doExchanging; //表示当前是否是在交易
			void doBuying(); //玩家确认后买入东西
			void doSaling(); //玩家确认后卖出东西

		private://GUI
			GUI::GUIButton* m_button_top_buy; //顶层的"买东西"按钮
			GUI::GUIButton* m_button_top_sale; //顶层的"卖东西"按钮
			GUI::GUIButton* m_button_top_say; //顶层的"谈话"按钮
			RPG_NS_Image::RPG_Image* m_back; //背景
			GUI::GUIButton* m_button_left; //向左选择键
			GUI::GUIButton* m_button_right; //向右选择键
			GUI::GUILabel* m_label_name; //显示"武器","药品","首饰"
			GUI::GUIListBox* m_listbox_items; //显示物品列表
			GUI::GUILabel* m_label_desc; //显示对物品的描述
			GUI::GUILabel* m_label_cost; //显示物品的价格
			GUI::GUILabel* m_label_count; //显示玩家拥有物品的数量
			GUI::GUIButton* m_button_ack; //"买","卖" 按钮
			GUI::GUIButton* m_button_exit; //退出按钮
			GUI::GUIMessageBox* m_messagebox; //确认信息
			GUI::GUILabel* m_label_money;
			void onTopBuy();
			void onTopSale();
			void onTopSay();
			void onLeft();
			void onRight();
			void onAck();
			void onExit();

			static void TopBuyHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onTopBuy();
			}
			static void TopSaleHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onTopSale();
			}
			static void TopSayHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onTopSay();
			}
			static void LeftHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onLeft();
			}
			static void RightHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onRight();
			}
			static void AckHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onAck();
			}
			static void ExitHelper(GUI::GUIObject* object){
				((RPG_ShopingSystem*)object)->onExit();
			}
				
			void initInterface(int interfaceID); //初始化相应的界面

		public:
			RPG_ShopingSystem(int mapID, int unicode); //构造买卖系统, 物品文件为fileName
			//~RPG_ShopingSystem();
			static void render(SDL_Surface* destSurf); //每周期执行

	};
}



#endif