#ifndef HEADER_GUI
#define HEADER_GUI

#include <SDL\SDL.h>
#include <SDL\SDL_ttf.h>
#include <string>
#include <vector>
#include "RPG_EventManager.h"
#include "RPG_Image.h"
#include "font.h"
using RPG_NS_EventManager::RPG_EventHandleInterface;
using RPG_NS_EventManager::RPG_EventManager;
using RPG_NS_Image::RPG_Image;
using std::string;
using std::vector;

/****
GUIButton触发外部事件的原理:
    
****/

namespace GUI
{
	void DrawPixel(SDL_Surface *screen,int x,int y,Uint8 R,Uint8 G,Uint8 B);
	void DrawLine (SDL_Surface* screen , int x1 , int y1 , int x2 , int y2 , Uint8 R, Uint8 G, Uint8 B);
	void DrawRect(SDL_Surface* screen, int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B, int thick);

	class GUIObject{
	public:virtual void draw(SDL_Surface* destSurf){};
	}; //便于渲染管理
	typedef void (*FuncType)(GUIObject* object);
	typedef void (*FuncTypeEx)(GUIObject* object, int id);

	class GUIButton: public RPG_EventHandleInterface, public GUIObject
	{ 
		private:
			GUIObject* m_pParent;
			int m_id; //按钮标记
			FuncType m_fClickEvent; //鼠标按下后松开的事件处理
			FuncType m_fOverEvent; //鼠标移动到按钮上时的事件处理
			FuncType m_fOutEvent; //鼠标离开按钮时的事件处理
			FuncTypeEx m_fClickEventEx; //鼠标按下后松开的事件处理
			FuncTypeEx m_fOverEventEx; //鼠标移动到按钮上时的事件处理
			FuncTypeEx m_fOutEventEx; //鼠标离开按钮时的事件处理
			void onClickEvent();
			void onOverEvent();
			void onOutEvent();
	
		private:
			int m_x, m_y; //按钮的位置
			int m_w, m_h; //按钮的大小
			RPG_Image* m_upImage; //正常状态下的按钮位图
			RPG_Image* m_downImage; //按下按钮时的位图
			int m_switch; //当前画图时要画哪张位图
			bool m_isInButton; //当前鼠标是否在Button上

		//Button类需要监听的事件
		private:
			virtual void onLButtonDown(int mx, int my);
			virtual void onLButtonUp(int mx, int my);
			virtual void onMouseMove(int mx, int my);
			bool inside(int x, int y){
				return x>m_x && x<m_x+m_w && y>m_y && y<m_y+m_h;
			}

		public:
			GUIButton(string upImagef, string downImagef, int x, int y, GUIObject* pParent, int id);
			GUIButton(string upImagef, string downImagef, int x, int y, GUIObject* pParent);
			GUIButton(string upImagef, string downImagef, GUIObject* pParent);
			~GUIButton();

			void setfClickEvent(FuncType f) {m_fClickEvent = f;}
			void setfOverEvent(FuncType f) {m_fOverEvent = f;}
			void setfOutEvent(FuncType f) {m_fOutEvent = f;}
			void setfClickEventEx(FuncTypeEx f) {m_fClickEventEx = f;}
			void setfOverEventEx(FuncTypeEx f) {m_fOverEventEx = f;}
			void setfOutEventEx(FuncTypeEx f) {m_fOutEventEx = f;}
			void draw(SDL_Surface* destSurf);
	};

	class GUILabel: public GUIObject
	{
		private:
			static int LabelCount; //该类的实例个数
			int m_x, m_y; //第一个文字的左上角
			int m_height; //文字的高度
			TTF_Font* m_pFont; //字体
			vector<SDL_Surface*> m_textSurface; //多行文字，每行文字的表面

		private:
			vector<string> splitString(string strSrc, string strSep);
			
		public:
			GUILabel(const string& font, int size, const string& message, 
				int r, int g, int b, int x, int y);
			GUILabel(const string& font, int size, const string& message, 
				int r, int g, int b);
			~GUILabel();
			
			int getHeight() {return m_height;}
			void setPos(int x, int y) {m_x = x; m_y = y;}
			void draw(SDL_Surface* destSurf);
	};

	class GUIMessageBox: public GUIObject
	{
		public:
			static const int MB_NULL = -1;
			static const int MB_YES = 0;
			static const int MB_NO = 1;

		private:
			RPG_Image* m_dlg; //对话框本身的背景图片
			GUILabel* m_caption; //标题栏
			GUILabel* m_message; //信息内容
			GUIButton* m_yes; //yes按钮的外观
			GUIButton* m_no; //no按钮的外观
			int m_x, m_y;
			int m_return;
			void freeControls();
			void onYesClick(void);
			void onNoClick(void);
			static void onYesClickHelper(GUIObject* object) {((GUIMessageBox*)object)->onYesClick();}
			static void onNoClickHelper(GUIObject* object) {((GUIMessageBox*)object)->onNoClick();}

		public:
			GUIMessageBox(string caption, string message, string dlg, string yes1, 
				string yes2, string no1, string no2, int x, int y, string font);
			~GUIMessageBox();

			void draw(SDL_Surface* destSurf);
			int getSelection() {return m_return;}
	};

	class GUIListBox: public RPG_EventHandleInterface, public GUIObject
	{
		private:
			int m_x, m_y;
			int m_w, m_h;
			int m_num; //列表可以显示的最多项数
			int m_topNum; //列表显示时当前最顶端的列表项编号
			int m_fontH; //字体的高度
			vector<GUILabel*> m_itemList; //存放列表项
			vector<string> m_itemValue; //存放列表项的内容
			GUIButton* m_toUp;
			GUIButton* m_toDown;
			int m_selectItem;

		private:
			static void onToUpClickHelper(GUIObject* object){
				((GUIListBox*)object)->onToUpClick();
			}
			static void onToDownClickHelper(GUIObject* object){
				((GUIListBox*)object)->onToDownClick();
			}
			void onToUpClick(); //向上
			void onToDownClick(); //向下
			void onLButtonDown(int mx, int my); //处理点击选项的事件
			void onKeyDown(SDLKey sym); //键盘，上，下键
			bool inside(int x, int y) {
				return x>m_x && x<m_x+m_w && y>m_y && y<m_y+m_h;
			}

		public:
			GUIListBox(int x, int y, int w, int num);
			~GUIListBox();

			int getSelectItem() {return m_selectItem;}
			string getSelectValue() {
				if(m_selectItem<0)return "";
				else return m_itemValue[m_selectItem];
			}
			void addItem(string value);
			void draw(SDL_Surface* destSurf);
	};

	class GUIProgressBar: public GUIObject
	{
		private:
			double m_radio; //当前进度条的百分比
			int m_x, m_y;
			int m_w, m_h;
			Uint8 m_R, m_G, m_B;
		public:
			GUIProgressBar(int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B);
			~GUIProgressBar();

			void setRadio(double radio) {m_radio = radio;}
			void draw(SDL_Surface* destSurf);
	};

	class GUIRenderManager
	{
		private:
			static vector<GUIObject*> m_renderList; //需要渲染得GUI对象

		public:
			static void guiRender(SDL_Surface* destSurf);
			static void addRender(GUIObject* object);
			static void delRender(GUIObject* object);
	};
}

#endif