#include <string.h>
#include <stdio.h>
#include <sstream>
#include "../include/RPG_TaskSystem.h"
#include "../include/game.h"

using RPG_NS_TaskSystem::ChapterInfo;
using RPG_NS_TaskSystem::TaskInfo;
using RPG_NS_TaskSystem::RPG_TaskSystem;
using std::stringstream;

/*******************************************/
vector<TaskInfo> RPG_TaskSystem::m_taskList; //对所有任务的描述
vector<ChapterInfo> RPG_TaskSystem::m_chapterList; //每章的任务情况
int RPG_TaskSystem::m_chapterCount; //当前章节数量
int RPG_TaskSystem::m_currentChapter; //当前章节
int RPG_TaskSystem::status; //当前的状态
RPG_TaskSystem* RPG_TaskSystem::instance;

/*******************************************/

void RPG_TaskSystem::RPG_TaskSystem_Init(){
	char buf[256];
	
	m_chapterCount = 0; //无章节
	m_currentChapter = 0; //默认第一章节
	FILE* fp;
	fp = fopen("data\\task\\all.task", "r");
	if(fp==NULL)throw "打不开文件：data\\task\\all.task";
	fscanf(fp, "%s", buf);
	while(strcmp(buf, "END")){
		TaskInfo t;
		t.id = string(buf); //id
		fscanf(fp, "%s", buf);
		while(strcmp(buf, "~")){//desc简述
			t.desc += string(buf);
			fscanf(fp, "%s", buf);
		}
		fscanf(fp, "%s", buf); 
		while(strcmp(buf, "~")){//详细描述
			t.detail += string(buf);
			fscanf(fp, "%s", buf);
		}
		m_taskList.push_back(t);
		fscanf(fp, "%s", buf);
	}
	
	/***********************/
	status = TASK_NULL;
	instance = NULL;
	/***********************/
	fclose(fp);
}

RPG_TaskSystem::RPG_TaskSystem(int chapter){
	m_currentChapter = chapter;
	m_button_Left = new GUI::GUIButton("image\\gui\\task\\taskLeft1.bmp",
		"image\\gui\\task\\taskLeft2.bmp",283, 0, this);
	m_button_Right = new GUI::GUIButton("image\\gui\\task\\taskRight1.bmp",
		"image\\gui\\task\\taskRight2.bmp", 483, 0, this);
	m_button_Exit = new GUI::GUIButton("image\\gui\\task\\taskExit1.bmp",
		"image\\gui\\task\\taskExit2.bmp", 362, 550, this);

	m_label_main = new GUI::GUILabel("font\\simkai.ttf", 19, "主线任务", 255, 0, 0, 50, 40);
	m_label_branch = new GUI::GUILabel("font\\simkai.ttf", 19, "支线任务", 0, 0, 255, 50, 295);

	m_back = new RPG_NS_Image::RPG_Image("image\\gui\\task\\back.bmp");
	m_label_chapterName = NULL;
	m_message = NULL;
	m_bMessageOn = false;

	createInterface(chapter); //显示该章的任务

	status = TASK_LIVE; //激活
	gamePause = true; //暂停游戏
	instance = this;

	//添加事件
	m_button_Left->setfClickEvent(onLeftHelper);
	m_button_Right->setfClickEvent(onRightHelper);
	m_button_Exit->setfClickEvent(onExitHelper);
}

RPG_TaskSystem::~RPG_TaskSystem(){
	delete m_button_Left;
	delete m_button_Right;
	delete m_button_Exit;
	delete m_label_main;
	delete m_label_branch;
	delete m_label_chapterName;
	delete m_back;
	for(int i=0; i<m_label_list.size(); i++){
		delete m_label_list[i];
		delete m_label_comp[i];
		delete m_button_detail[i];
	}
	m_label_list.clear();
	m_label_comp.clear();
	m_button_detail.clear();
}

TaskInfo RPG_TaskSystem::getTaskInfo(string id){
	for(int i=0; i<m_taskList.size(); i++){
		if(id.compare(m_taskList[i].id)==0)
			return m_taskList[i];
	}
}

void RPG_TaskSystem::createInterface(int chapter){
	if(chapter<0 || chapter>=m_chapterCount)return;
	/***************删除过时的ＧＵＩ******************/
	for(int i=0; i<m_label_list.size(); i++){
		delete m_label_list[i];
		delete m_label_comp[i];
		delete m_button_detail[i];
	}
	m_label_list.clear();
	m_label_comp.clear();
	m_button_detail.clear();
	if(m_label_chapterName!=NULL)delete m_label_chapterName;
	/************************************************/
	//只有画主线任务简述跟支线任务简述
	int mainCount=0, branchCount=0;
	TaskInfo t;
	GUI::GUILabel* label;
	GUI::GUIButton* button;
	stringstream stream;
	stream << "第   ";
	stream << chapter+1;
	stream << "   章";
	m_label_chapterName = new GUI::GUILabel("font\\simkai.ttf", 19, stream.str(),
		0, 0, 0, 342, 3);
	if(m_chapterList.size()==0)return; //还没有章节
	for(int i=0; i<m_chapterList[chapter].id.size(); i++){
		t = getTaskInfo(m_chapterList[chapter].id[i]);
		if(t.id.find_first_of("m")!=-1){//主线
			label = new GUI::GUILabel("font\\simkai.ttf", 15,
				t.desc, 100, 100, 100, 50, 80+mainCount*25);
			m_label_list.push_back(label);//简述
			if(m_chapterList[chapter].comp[i]){
				label = new GUI::GUILabel("font\\simkai.ttf", 15,
					"已完成", 0, 255, 0, 600, 80+mainCount*25);
			}
			else{
				label = new GUI::GUILabel("font\\simkai.ttf", 15,
					"未完成", 255, 0, 0, 600, 80+mainCount*25);
			}
			m_label_comp.push_back(label);//完成情况
			button = new GUI::GUIButton("image\\gui\\task\\taskDetail1.bmp",
				"image\\gui\\task\\taskDetail2.bmp", 680, 80+mainCount*25-3, this, i);
			button->setfClickEventEx(onDetailHelper); //添加事件
			m_button_detail.push_back(button); //详细表述按钮
			mainCount++;
		}else{//支线
			label = new GUI::GUILabel("font\\simkai.ttf", 15,
				t.desc, 100, 100, 100, 50, 335+branchCount*25);
			m_label_list.push_back(label);//简述
			if(m_chapterList[chapter].comp[i]){
				label = new GUI::GUILabel("font\\simkai.ttf", 15,
					"已完成", 0, 255, 0, 600, 335+branchCount*25);
			}
			else{
				label = new GUI::GUILabel("font\\simkai.ttf", 15,
					"未完成", 255, 0, 0, 600, 335+branchCount*25);
			}
			m_label_comp.push_back(label);//完成情况
			button = new GUI::GUIButton("image\\gui\\task\\taskDetail1.bmp",
				"image\\gui\\task\\taskDetail2.bmp", 680, 335+branchCount*25-3, this, i);
			button->setfClickEventEx(onDetailHelper); //添加事件
			m_button_detail.push_back(button); //详细表述按钮
			branchCount++;
		}
	}
}


/************************按钮处理*******************************/
void RPG_TaskSystem::onLeft(){
	if(m_bMessageOn)return;
	if(m_currentChapter==0)return;
	m_currentChapter--;
	createInterface(m_currentChapter);
}

void RPG_TaskSystem::onRight(){
	if(m_bMessageOn)return;
	if(m_currentChapter==m_chapterCount-1)return;
	m_currentChapter++;
	createInterface(m_currentChapter);
}

void RPG_TaskSystem::onExit(){
	if(m_bMessageOn)return;
	status = TASK_DEAD;
}

void RPG_TaskSystem::onDetail(int id){
	if(m_bMessageOn)return;
	m_bMessageOn = true;
	string taskId = m_chapterList[m_currentChapter].id[id];
	TaskInfo t = getTaskInfo(taskId);
	t.detail = newLine(t.detail, 15);
	m_message = new GUI::GUIMessageBox("任务详情", t.detail, "image\\gui\\task\\taskMessageBoxBack.bmp",
		"image\\gui\\task\\taskMessageBoxYes1.bmp", "image\\gui\\task\\taskMessageBoxYes2.bmp",
		"image\\gui\\task\\taskMessageBoxNo1.bmp", "image\\gui\\task\\taskMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
}
/**************************************************************/

void RPG_TaskSystem::render(SDL_Surface* destSurf){
	if(status==TASK_NULL)return;
	if(status==TASK_DEAD){
		delete instance;
		instance = NULL;
		status = TASK_NULL;
		gamePause = false;
		return;
	}
	//激活状态
	instance->m_back->drawImage(destSurf, 0, 0);
	instance->m_button_Left->draw(destSurf);
	instance->m_button_Right->draw(destSurf);
	instance->m_button_Exit->draw(destSurf);
	instance->m_label_main->draw(destSurf);
	instance->m_label_branch->draw(destSurf);
	if(m_chapterList.size()==0)return;
	instance->m_label_chapterName->draw(destSurf);
	for(int i=0; i<instance->m_label_list.size(); i++){
		instance->m_label_list[i]->draw(destSurf);
		instance->m_label_comp[i]->draw(destSurf);
		instance->m_button_detail[i]->draw(destSurf);
	}
	if(instance->m_message!=NULL){//出现详细描述信息框
		if(instance->m_message->getSelection()!=GUI::GUIMessageBox::MB_NULL){//按下了yes或no
			delete instance->m_message;
			instance->m_message = NULL;
			instance->m_bMessageOn = false;
		}
		else{
			instance->m_message->draw(destSurf);
		}
	}
}

void RPG_TaskSystem::addChapter(){
	ChapterInfo t;
	m_chapterCount++;
	m_chapterList.push_back(t);
}

void RPG_TaskSystem::receive(string id){
	int index = id.find_first_of("_");
	string strC = id.substr(7, index-7);
	int chapter;
	sscanf(strC.c_str(), "%d", &chapter);
	chapter--; //索引从０开始
	m_chapterList[chapter].id.push_back(id);
	m_chapterList[chapter].comp.push_back(false); //“未完成”状态
}

void RPG_TaskSystem::complete(string id){
	int index = id.find_first_of("_");
	string strC = id.substr(7, index-7);
	int chapter;
	sscanf(strC.c_str(), "%d", &chapter);
	chapter--; //索引从０开始
	for(int i=0; i<m_chapterList[chapter].comp.size(); i++){
		if(m_chapterList[chapter].id[i].compare(id)==0)
			m_chapterList[chapter].comp[i] = true; //完成
	}
}
