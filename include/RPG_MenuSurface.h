#ifndef HEADER_MENUSURFACE_H
#define HEADER_MENUSURFACE_H
/*
	游戏界面
*/
#include "GUI.h"
#include "game.h"
#include "RPG_Image.h"
#include "SDL\SDL.h"

namespace RPG_NS_MenuSurface{
	class RPG_MenuSurface : public GUI::GUIObject 
	{
	private:
		static const int STUTAS_X = 320;
		static const int STUTAS_Y = 470;
		static const int STUTAS_FONT = 20;

	private:
		static bool m_ableSurface;//是否允许界面，（谈话系统触发时失效）
		static int m_numOfhero; //英雄个数
		static RPG_MenuSurface* instance;
		static RPG_NS_Image::RPG_Image* m_bk;//菜单条背景
		GUI::GUIButton* m_hero0; //英雄选择按钮
		static GUI::GUIButton* m_hero1;
		static GUI::GUIButton* m_hero2;
		GUI::GUIButton* m_loaddlg;//调出载入进度对话框
		GUI::GUIButton* m_savedlg;//保存进度对话框
		GUI::GUIButton* m_exitdlg;//提出游戏
		GUI::GUIButton* m_taskdlg;//查看任务
		GUI::GUIButton* m_equipmentdlg;//调出装备设置对话框
		GUI::GUILabel* m_status;//游戏辅助菜单帮助信息显示
		GUI::GUILabel* m_monery;//游戏金钱数
		//弹出英雄信息
		RPG_NS_Image::RPG_Image* m_heroP;//英雄全身像
		RPG_NS_Image::RPG_Image* m_heroShowB;//状态显示框
		GUI::GUILabel* m_hp;//血量（文字显示）
		GUI::GUILabel* m_mana;//魔法
		GUI::GUILabel* m_cp;//聚齐
		GUI::GUILabel* m_name;//英雄名字
		GUI::GUILabel* m_level;//英雄等级
		GUI::GUILabel* m_exp;//英雄经验
		GUI::GUIProgressBar* m_hppb;//血条
		GUI::GUIProgressBar* m_manapb;
		GUI::GUIProgressBar* m_cppb;
		static int m_counter;

	public:
		static void RPG_MenuSurface_Init();//初始化菜单界面 
		//按钮单击事件
		void onHero0();
		void onHero1();
		void onHero2();
		void onLoad();
		void onSave();
		void onExit();
		void onTask();
		void onEquipment();
		//按钮onover事件，在状态栏显示帮助信息
		void overHero0();
		void overHero1();
		void overHero2();
		void overLoad();
		void overSave();
		void overExit();
		void overTask();
		void overEquipment();
		//按钮onout事件
		void outHero0();
		void outHero1();
		void outHero2();
		void outLoad();
		void outSave();
		void outExit();
		void outTask();
		void outEquipment();
	public:
		static void Hero0_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onHero0();
		}
		static void Hero1_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onHero1();
		}
		static void Hero2_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onHero2();
		}
		static void Load_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onLoad();
		}
		static void Save_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onSave();
		}
		static void Exit_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onExit();
		}
		static void Task_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onTask();
		}
		static void Equipment_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->onEquipment();
		}
		//
		static void O_Hero0_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overHero0();
		}
		static void O_Hero1_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overHero1();
		}
		static void O_Hero2_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overHero2();
		}
		static void O_Load_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overLoad();
		}
		static void O_Save_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overSave();
		}
		static void O_Exit_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overExit();
		}
		static void O_Task_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overTask();
		}
		static void O_Equipment_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->overEquipment();
		}
		//
		static void Ot_Hero0_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outHero0();
		}
		static void Ot_Hero1_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outHero1();
		}
		static void Ot_Hero2_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outHero2();
		}
		static void Ot_Load_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outLoad();
		}
		static void Ot_Save_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outSave();
		}
		static void Ot_Exit_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outExit();
		}
		static void Ot_Task_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outTask();
		}
		static void Ot_Equipment_Helper(GUI::GUIObject* object){
			if(!m_ableSurface || gamePause)
				return;
			((RPG_MenuSurface*)object)->outEquipment();
		}
	public:
		static void enableSurface(bool state) {m_ableSurface = state;}
		static void setNumOfher(int num) {m_numOfhero = num;}
		static void render(SDL_Surface *);//
		static void createHeroshow(int heroID);
		static void showHeroUpdate(SDL_Surface* pScreen);

		RPG_MenuSurface(void);
		~RPG_MenuSurface(void);
	};
}

#endif