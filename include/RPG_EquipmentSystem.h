#ifndef HEADER_RPG_EQUIPMENTSYSTEM
#define HEADER_RPG_EQUIPMENTSYSTEM

/***********
*******人物装备分类及可以装备的数量
武器类:
头盔:1个 编号0
衣服:1个 编号1
鞋子:1个 编号2
武器:1个 编号3
盾牌:1个 编号4
头巾:1个 编号5
腰带:1个 编号6

首饰类:
耳环:2个 编号7,8
项链:1个 编号9
手镯:2个 编号10,11
戒指:1个 编号12
护身符:3个 编号13,14,15

***********/
#include <vector>
#include <string>
#include <SDL\SDL.h>
#include "game.h"
#include "GUI.h"
using std::vector;
using std::string;

namespace RPG_NS_EquipmentSystem
{
	class RPG_EquipmentSystem:public GUI::GUIObject
	{
		public:
			static const int EQP_TOUKUI = 0;
			static const int EQP_YIFU = 1;
			static const int EQP_XIEZI = 2;
			static const int EQP_WUQI = 3;
			static const int EQP_DUNPAI = 4;
			static const int EQP_TOUJING = 5;
			static const int EQP_YAODAI = 6;
			static const int EQP_ERHUAN = 7;
			static const int EQP_XIANGLIAN = 9;
			static const int EQP_SHOUZHUO = 10;
			static const int EQP_JIEZHI = 12;
			static const int EQP_HUSHENFU = 13;

			static const int EQP_NULL = 0; //没有实例
			static const int EQP_LIVE = 1; //存活实例
			static const int EQP_DEAD = 2; //死亡,需要删除
		
		private:
			static RPG_EquipmentSystem* instance;
			static int status;
			int m_heroIndex; //当前显示的英雄
			vector<Item> m_itemList; //所有物品的详细信息
			

			string intToString(int n);
			/*******************按钮处理**********************/
			void onLeft();
			void onRight();
			void onUninstall();
			void onInstall();
			void onDrink();
			void onExit();
			static void onLeftHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onLeft();
			}
			static void onRightHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onRight();
			}
			static void onUninstallHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onUninstall();
			}
			static void onInstallHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onInstall();
			}
			static void onDrinkHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onDrink();
			}
			static void onExitHelper(GUIObject* object){
				((RPG_EquipmentSystem*)object)->onExit();
			}
			/*************************************************/
		private://GUI
			static string equipLabel[16];
			static string statLabel[5];
			string m_hasEquip[16]; //16个装备位置的各个装备名称
			
			//按钮
			GUI::GUIButton* m_button_left; //向左按钮
			GUI::GUIButton* m_button_right; //向右按钮
			GUI::GUIButton* m_button_uninstall; //卸载装备
			GUI::GUIButton* m_button_install; //装备上去
			GUI::GUIButton* m_button_drink; //喝药
			//图片
			RPG_NS_Image::RPG_Image* m_image_back; //背景图片
			RPG_NS_Image::RPG_Image* m_image_hero; //英雄的图片
			//进度条
			GUI::GUIProgressBar* m_progressbar_blood; //血
			GUI::GUIProgressBar* m_progressbar_mana; //魔
			GUI::GUIProgressBar* m_progressbar_cp; //CP
			//列表框
			GUI::GUIListBox* m_listbox_hequip; //英雄身上的装备列表
			GUI::GUIListBox* m_listbox_pequip; //包裹中的装备列表
			GUI::GUIListBox* m_listbox_drug; //包裹中的药品列表
			//标签框
			/**************************************/
			GUI::GUILabel* m_label_name; //人物名称
			GUI::GUILabel* m_label_level; //人物等级
			GUI::GUILabel* m_label_experience; //经验,下一级经验
			GUI::GUILabel* m_label_blood; //血,百分比
			GUI::GUILabel* m_label_mana; //魔,百分比
			GUI::GUILabel* m_label_cp; //CP,百分比
			GUI::GUILabel* m_label_attribute[5]; //物理攻击,物理防御,魔法攻击,魔法防御,速度信息
			GUI::GUILabel* desc1, *desc2, *desc3;
			/**************************************/
			GUI::GUILabel* m_label_equipLabel[16];//装备的种类标签
			/**************************************/
			GUI::GUIButton* m_button_exit; //回到游戏界面的按钮


			void showHero(int index); //显示英雄的相关信息	
			Item getItemInfo(string material);//获得名称为material的物品详细信息
			void delAllGUI(); //删除除了不变得那些控件之外的GUI控件
			void setNULL();

		public:
			RPG_EquipmentSystem();
			~RPG_EquipmentSystem();
			static void render(SDL_Surface* destSurf);
		
	};
}


#endif