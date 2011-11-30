/*****************
**********data/task/all.task
chapter1_main_1 ; 表示第一章主线第一个任务
简述
~
详细描述
~
chapter1_branch_1 ; 表示第一章支线第一个任务
简述
~
详细描述
~
END ;表示结束
*****************/

#ifndef HEADER_RPG_TASKSYSTEM
#define HEADER_RPG_TASKSYSTEM

#include <vector>
#include <SDL/SDL.h>
#include "GUI.h"
#include "RPG_Image.h"
#include "game.h"
using std::vector;

namespace RPG_NS_TaskSystem
{
	typedef struct{
		string id; //任务id
		string desc; //任务简述
		string detail; //任务详细描述
	}TaskInfo; //任务的信息

	typedef struct{
		vector<string> id; //本章已经接受的任务列表
		vector<bool> comp; //对应的任务完成情况
	}ChapterInfo; //每章的当前任务信息

	class RPG_TaskSystem: public GUI::GUIObject
	{
		public:
			static const int TASK_NULL = 0;
			static const int TASK_LIVE = 1;
			static const int TASK_DEAD = 2;

		private://GUI
			GUI::GUILabel* m_label_main; //显示“主线任务“的标签
			GUI::GUILabel* m_label_branch; //显示“支线任务”的标签
			GUI::GUILabel* m_label_chapterName; //章节名称
			GUI::GUIButton* m_button_Left; //章节的选项按钮,向左
			GUI::GUIButton* m_button_Right; //章节的选项按钮，向右
			GUI::GUIButton* m_button_Exit; //推出按钮
			vector<GUI::GUILabel*> m_label_list; //当前显示任务信息的标签
			vector<GUI::GUILabel*> m_label_comp; //对应的任务完成情况
			vector<GUI::GUIButton*> m_button_detail; //详细情况查看按钮
			RPG_NS_Image::RPG_Image* m_back; //背景图片
			GUI::GUIMessageBox* m_message; //详细描述框
			bool m_bMessageOn; //信息框是否出现

			void onLeft();
			void onRight();
			void onExit();
			void onDetail(int id); //详细情况
			static void onLeftHelper(GUIObject* object){
				((RPG_TaskSystem*)object)->onLeft();
			}
			static void onRightHelper(GUIObject* object){
				((RPG_TaskSystem*)object)->onRight();
			}
			static void onExitHelper(GUIObject* object){
				((RPG_TaskSystem*)object)->onExit();
			}
			static void onDetailHelper(GUIObject* object, int id){
				((RPG_TaskSystem*)object)->onDetail(id);
			}	

		private:
			static vector<TaskInfo> m_taskList; //对所有任务的描述
			static vector<ChapterInfo> m_chapterList; //每章的任务情况
			static TaskInfo getTaskInfo(string id); //根据id得到任务的具体情况
			static int m_chapterCount; //当前章节数量
			static int m_currentChapter; //当前章节
			static int status; //当前的状态

			static RPG_TaskSystem* instance;

		public:
			RPG_TaskSystem(int chapter);
			~RPG_TaskSystem();
			static void RPG_TaskSystem_Init(); //主要读入任务描述
			void createInterface(int chapter); //生成章节chapter的部分
			static void render(SDL_Surface* destSurf);

			static void receive(string id); //接收到一个任务“未完成”状态
			static void complete(string id); //完成一个任务“已经完成”状态
			static void addChapter(); //增加一章
	};
}

#endif HEADER_RPG_TASKSYSTEM