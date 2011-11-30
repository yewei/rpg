#include "../include/RPG_DialogSystem.h"
#include "../include/RPG_SpriteManager.h"
#include "../include/RPG_SceneManager.h"
#include "../include/RPG_SoundManager.h"
#include "../include/RPG_MenuSurface.h"
#include "../include/RPG_NPC.h"
#include <sstream>
#include "../include/game.h"

using RPG_NS_SceneManager::RPG_SceneManager;
// *.say文件位于data\say目录下，取名规则,例：map1_dialog.say

int RPG_NS_DialogSystem::RPG_DialogSystem::status = DLG_NULL;
RPG_NS_DialogSystem::RPG_DialogSystem* RPG_NS_DialogSystem::RPG_DialogSystem::instance = NULL;
std::string RPG_NS_DialogSystem::RPG_DialogSystem::saypath = "data\\say\\";
std::string RPG_NS_DialogSystem::RPG_DialogSystem::picturePath = "image\\picture\\";
bool RPG_NS_DialogSystem::RPG_DialogSystem::saySentence = false;

RPG_NS_DialogSystem::RPG_DialogSystem::RPG_DialogSystem(int mapID, int npcUnicodeID, std::string* heroName){

	std::string dlgFile;
	dlgFile = "map" + intostring(mapID) + "_dialog.say";
	dlgFile = saypath + dlgFile;
	instance = this; //添加实例
	instance->m_sayFile = fopen(dlgFile.c_str(), "r");	//打开对话文件
	if(instance->m_sayFile == NULL){
		throw "can't find file" + dlgFile;
		return;
	}
	status = DLG_DEAD; //
	//
	int uid;
	char npcName[100];
	char reserve[256];
	while(fscanf(instance->m_sayFile, "%d %s %s", &uid, npcName, reserve) != EOF){
		if(uid == npcUnicodeID){	//搜索到
			instance->m_npcName = new GUI::GUILabel("font\\simkai.ttf", 20, npcName, 128, 20, 20, SAYNAME_X, SAYNAME_Y);	
			status = DLG_LIVE; //激活
			gamePause = true;//暂停玩家对英雄的控制和AI
			instance->m_back = new RPG_NS_Image::RPG_Image(picturePath+"dlgBK.bmp");
			break;
		}
		status = DLG_DEAD;
	}
	for(int inum=0; inum<3; inum++)
		instance->m_heroName[inum] = heroName[inum];
	//instance->m_say = NULL;
	instance->m_picture = NULL;
	instance->m_currentSay = NULL;
	instance->m_nameOfCurrentSay = NULL;
	instance->m_flag = false;
	//添加事件队列
	RPG_NS_EventManager::RPG_EventManager::addListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
	RPG_NS_EventManager::RPG_EventManager::addListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
}

RPG_NS_DialogSystem::RPG_DialogSystem::~RPG_DialogSystem(void){
	RPG_NS_EventManager::RPG_EventManager::delListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
	RPG_NS_EventManager::RPG_EventManager::delListen(this, RPG_NS_EventManager::RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
}

void RPG_NS_DialogSystem::RPG_DialogSystem::createDialog(int mapID, int npcUnicodeID){
	if(gamePause) return;
	std::string heroName[3];
	heroName[0] = heroSet.heros[0].heroName;
	heroName[1] = heroSet.heros[1].heroName;
	heroName[2] = heroSet.heros[2].heroName;
	new RPG_NS_DialogSystem::RPG_DialogSystem(mapID, npcUnicodeID, heroName); 
}

void RPG_NS_DialogSystem::RPG_DialogSystem::Update(SDL_Surface* destSurf){
	
	if(status==DLG_NULL)return;//无需执行
	if(status==DLG_DEAD){ //删除已经死亡的shop
		delete instance;
		instance = NULL;
		status = DLG_NULL;
		return;
	}
	//激活状态
	RPG_NS_MenuSurface::RPG_MenuSurface::enableSurface(false);//禁止菜单
	update(destSurf);
}

void RPG_NS_DialogSystem::RPG_DialogSystem::update(SDL_Surface* destSurf){

	static int uid;//-2~0:三英雄，其他为npc
	char pictureName[100];//头像文件路径
	char content[256];//说话内容
	//static bool saySentence = false;//是否在说某句话的过程中
	static std::string t_currentSay;
	static int nIndex;
	static int count=0;
	static bool oneDlgover = true;
	static bool firstIn = true;
	
	if(saySentence){//正在说某一句话
		count++;//计数器累加
		if(count < 3 && count != 1){
			//显示各个对话信息
			instance->m_back->drawImage(destSurf, 0, BK_Y);//贴对话框背景
			instance->m_nameOfCurrentSay->draw(destSurf);//显示说话者名字
			instance->m_picture->setColorKey(255, 0, 255);
			if(instance->m_picture != NULL)
				instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//显示头像
			instance->m_currentSay->draw(destSurf);//显示对话内容
		}
		else{
			count = 1;
			//
			oneDlgover = false;
			if(instance->m_currentSay != NULL){
				delete instance->m_currentSay;
				instance->m_currentSay = NULL;
			}
			if(getHZbyIndex(instance->m_content, nIndex).size()){
				t_currentSay = t_currentSay + getHZbyIndex(instance->m_content, nIndex);
				nIndex++;
				instance->m_currentSay = new GUI::GUILabel("font\\simkai.ttf", 20, t_currentSay, 
						64, 64, 64, SAYCONTENT_X, SAYCONTENT_Y);
				//
				//显示各个对话信息
				instance->m_back->drawImage(destSurf, 0, BK_Y);//贴对话框背景
				instance->m_nameOfCurrentSay->draw(destSurf);//显示说话者名字
				instance->m_picture->setColorKey(255, 0, 255);
				if(instance->m_picture != NULL)
					instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//显示头像
				instance->m_currentSay->draw(destSurf);//显示对话内容
			}
			else{//结束一句话
				count = 0;
				saySentence = false;
				if(instance->m_currentSay != NULL){
					delete instance->m_currentSay;
					instance->m_currentSay = NULL;
				}
				instance->m_currentSay = new GUI::GUILabel("font\\simkai.ttf", 20, t_currentSay, 
							64, 64, 64, SAYCONTENT_X, SAYCONTENT_Y);
				instance->m_back->drawImage(destSurf, 0, BK_Y);//贴对话框背景
				instance->m_nameOfCurrentSay->draw(destSurf);//显示说话者名字
				instance->m_picture->setColorKey(255, 0, 255);
				if(instance->m_picture != NULL)
					instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//显示头像
				instance->m_currentSay->draw(destSurf);//显示对话内容
			}
		}	
	}
	else{//
		//显示各个对话信息
		if(!oneDlgover){
			if(instance->m_currentSay != NULL){
				delete instance->m_currentSay;
				instance->m_currentSay = NULL;
			}
			instance->m_currentSay = new GUI::GUILabel("font\\simkai.ttf", 20, t_currentSay, 
						64, 64, 64, SAYCONTENT_X, SAYCONTENT_Y);
			instance->m_back->drawImage(destSurf, 0, BK_Y);//贴对话框背景
			instance->m_nameOfCurrentSay->draw(destSurf);//显示说话者名字
			instance->m_picture->setColorKey(255, 0, 255);
			if(instance->m_picture != NULL)
				instance->m_picture->drawImage(destSurf, PICTURE_X, PICTURE_Y);//显示头像
			instance->m_currentSay->draw(destSurf);//显示对话内容
		}
		if(firstIn) goto label;
		//说完一句话,等待鼠标左键或空格按下
		if(!instance->m_flag) return;
label:
		RPG_NS_SoundManager::RPG_SoundManager::playSnd(RPG_NS_SoundManager::RPG_SoundManager::WORDSND);//播放说话音效
		firstIn = false;
		instance->m_flag = false;
		if(instance->m_picture != NULL){
			delete instance->m_picture;
			instance->m_picture = NULL;
		}
		if(uid<=0 && instance->m_nameOfCurrentSay!=NULL){
			delete instance->m_nameOfCurrentSay;
			instance->m_nameOfCurrentSay = NULL;
		}
		fscanf(instance->m_sayFile, "%d %s %s", &uid, pictureName, content);//读一句话
		if(uid == -255){	//对话结束
			delete instance->m_back;
			delete instance->m_npcName;
			fclose(instance->m_sayFile);
			status = DLG_DEAD;
			oneDlgover = true;
			gamePause = false;
			firstIn = true;
			RPG_NS_SoundManager::RPG_SoundManager::clearEft();//清楚音效残留分配资源
			RPG_NS_MenuSurface::RPG_MenuSurface::enableSurface(true);//开启菜单
		}
		else{
			oneDlgover = false;
			t_currentSay = "";
			nIndex = 0;
			string temp = "NULL";
			count = 0;
			if(temp.compare(pictureName))//
				instance->m_picture = new RPG_NS_Image::RPG_Image(picturePath+pictureName);//生成头像
			instance->m_content = content;
			if(uid>0){//是npc
				instance->m_nameOfCurrentSay = instance->m_npcName;
			}
			else{//是英雄
				instance->m_nameOfCurrentSay = new GUI::GUILabel("font\\simkai.ttf", 20, instance->m_heroName[-1*uid], 
					128, 20, 20, SAYNAME_X, SAYNAME_Y);//生成叙述者名字
			}
			saySentence = true;
		}	
	}
}

string RPG_NS_DialogSystem::RPG_DialogSystem::intostring(int n){

	stringstream str;
	str << n;
	return str.str();
}

string RPG_NS_DialogSystem::RPG_DialogSystem::getHZbyIndex(std::string str, int index){
	
	int iNum = 0;
	std::string t_str;
	for(int i=0; i<str.size(); i++){
		if(str[i]>=0){
			t_str = str.substr(i, 1);
			if(index == iNum++) return t_str;
		}
		else{
			t_str = str.substr(i, 2);
			if(index == iNum++) return t_str;
			i++;
		}
	}
	t_str = "";
	return t_str;
}

//void RPG_NS_DialogSystem::RPG_DialogSystem::onRButtonDown(int mx, int my){
	
//}

void RPG_NS_DialogSystem::RPG_DialogSystem::onLButtonDown(int mx, int my){
	if(!saySentence){
		instance->m_flag = true;
	}
}

void RPG_NS_DialogSystem::RPG_DialogSystem::onKeyDown(SDLKey sym){
	if(!saySentence && sym==SDLK_SPACE){
		instance->m_flag = true;
	}
}