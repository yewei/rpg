#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "../include/RPG_LearnMagic.h"
using std::string;
using std::stringstream;

using RPG_NS_LearnMagic::RPG_LearnMagic;
using RPG_NS_LearnMagic::MagicItem;

RPG_LearnMagic* RPG_LearnMagic::instance = NULL;
int RPG_LearnMagic::status = LEARN_NULL;

RPG_LearnMagic::RPG_LearnMagic(int mapId, int unicode){
	//构造一个魔法商店
	string fileName;
	stringstream stream;
	stream << mapId;
	stream << "U";
	stream << unicode;
	fileName = "data\\learnMagic\\mapID"+stream.str()+".learn";
	FILE* fp = fopen(fileName.c_str(), "r");
	if(fp==NULL)throw "无法打开文件"+fileName;

	MagicItem t;
	while(!feof(fp)){
		fscanf(fp, "%s", t.magicName); //名称
		if(!strcmp(t.magicName, "END"))break;
		fscanf(fp, "%s", t.magicDesc); //描述
		fscanf(fp, "%d", &t.cost); //价格
		for(int i=0; i<4; i++){
			fscanf(fp, "%d", &t.soul[i]); //四魂
		}
		magicList.push_back(t);
	}
	fclose(fp);

	m_currentHero = 0;
	instance = this;
	status = LEARN_LIVE;
	gamePause = true;

	/////////////////不变的界面////////////////////
	m_back = new RPG_NS_Image::RPG_Image("image\\gui\\learnMagic\\back.bmp");
	m_message = NULL;

	m_label_name = NULL; //英雄姓名
	m_label_totSoul = NULL; //描述当前英雄总的四魂
	m_label_pointSoul = NULL; //剩余的点数
	m_label_soul[0] = NULL; //剩余魂1
	m_label_soul[1] = NULL; //剩余魂2
	m_label_soul[2] = NULL; //剩余点3
	m_label_soul[3] = NULL; //剩余点4
	m_listbox_heroMagic = NULL;
	
	m_label_desc = NULL;
	m_label_cost = NULL;
	m_label_needSoul = NULL;

	/////
	m_button_left = new GUI::GUIButton("image\\gui\\learnMagic\\learnLeft1.bmp",
		"image\\gui\\learnMagic\\learnLeft2.bmp", HEROBASEX+10, HEROBASEY+10, this);
	m_button_left->setfClickEvent(onLeftHelper);
	m_button_right = new GUI::GUIButton("image\\gui\\learnMagic\\learnRight1.bmp",
		"image\\gui\\learnMagic\\learnRight2.bmp", HEROBASEX+155, HEROBASEY+10, this);
	m_button_right->setfClickEvent(onRightHelper);
	m_button_add1 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+125, this);
	m_button_add1->setfClickEvent(onAdd1Helper);
	m_button_add2 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+150, this);
	m_button_add2->setfClickEvent(onAdd2Helper);
	m_button_add3 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+175, this);
	m_button_add3->setfClickEvent(onAdd3Helper);
	m_button_add4 = new GUI::GUIButton("image\\gui\\learnMagic\\learnAdd1.bmp",
		"image\\gui\\learnMagic\\learnAdd2.bmp", HEROBASEX+180, HEROBASEY+200, this);
	m_button_add4->setfClickEvent(onAdd4Helper);

	m_label_zsh = new GUI::GUILabel("font\\simkai.ttf", 17, "四魂：",
		0, 0, 0, HEROBASEX+10, HEROBASEY+55);

	/////
	stream.str("");
	stream << "$";
	stream << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 15, stream.str(),
		255, 255, 128, LEARNBASEX+45, LEARNBASEY+342);
	m_button_exit = new GUI::GUIButton("image\\gui\\learnMagic\\learnExit1.bmp",
		"image\\gui\\learnMagic\\learnExit2.bmp", LEARNBASEX+472, LEARNBASEY+12, this);
	m_button_exit->setfClickEvent(onExitHelper);
	m_button_buy = new GUI::GUIButton("image\\gui\\learnMagic\\learnBuy1.bmp",
		"image\\gui\\learnMagic\\learnBuy2.bmp", LEARNBASEX+180, LEARNBASEY+342, this);
	m_button_buy->setfClickEvent(onBuyHelper);

	m_listbox_learnMagic = new GUI::GUIListBox(LEARNBASEX+30, LEARNBASEY+35, 165, 7);
	for(int i=0; i<magicList.size(); i++){
		m_listbox_learnMagic->addItem(magicList[i].magicName);
	}

	////////////////变化的界面/////////////////////
	createHeroInterface(0); //默认第一个英雄
}

RPG_LearnMagic::~RPG_LearnMagic(){
	if(m_button_exit!=NULL)delete m_button_exit;
	if(m_button_buy!=NULL)delete m_button_buy;
	if(m_button_left!=NULL)delete m_button_left;
	if(m_button_right!=NULL)delete m_button_right;
	if(m_button_add1!=NULL)delete m_button_add1;
	if(m_button_add2!=NULL)delete m_button_add2;
	if(m_button_add3!=NULL)delete m_button_add3;
	if(m_button_add4!=NULL)delete m_button_add4;

	if(m_listbox_learnMagic!=NULL)delete m_listbox_learnMagic;
	if(m_listbox_heroMagic!=NULL)delete m_listbox_heroMagic;
	
	if(m_label_desc!=NULL)delete m_label_desc;
	if(m_label_cost!=NULL)delete m_label_cost;
	if(m_label_needSoul!=NULL)delete m_label_needSoul;
	if(m_label_money!=NULL)delete m_label_money;
	if(m_label_name!=NULL)delete m_label_name;
	if(m_label_zsh!=NULL)delete m_label_zsh;
	if(m_label_totSoul!=NULL)delete m_label_totSoul;
	if(m_label_pointSoul!=NULL)delete m_label_pointSoul;
	for(int i=0; i<4; i++)if(m_label_soul[i]!=NULL)
		delete m_label_soul[i];

	gamePause = false;
}



void RPG_LearnMagic::createHeroInterface(int index){

	//删除过时的GUI
	m_currentHero = index;
	if(m_label_name!=NULL)delete m_label_name; //英雄姓名
	if(m_label_totSoul!=NULL)delete m_label_totSoul; //描述当前英雄总的四魂
	if(m_label_pointSoul!=NULL)delete m_label_pointSoul; //剩余的点数
	for(int i=0; i<4; i++)if(m_label_soul[i]!=NULL)
		delete m_label_soul[i];

	if(m_listbox_heroMagic!=NULL)delete m_listbox_heroMagic; //英雄的已有魔法

	//新建GUI
	stringstream stream;
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 19, string(heroSet.heros[index].heroName),
		0, 0, 0, HEROBASEX+60, HEROBASEY+20);
	for(int i=1; i<=4 ;i++){ //每个魂的总点数
		stream << "魂";
		stream << i;
		stream << ": ";
		stream << heroSet.heros[index].totSoul[i-1];
		stream << " ";
	}
	m_label_totSoul = new GUI::GUILabel("font\\simkai.ttf", 12, stream.str(), 
		0, 0, 0, HEROBASEX+10, HEROBASEY+80);
	stream.str("");
	stream << "剩余点数：";
	stream << heroSet.heros[index].pointSoul;
	m_label_pointSoul = new GUI::GUILabel("font\\simkai.ttf", 17, stream.str(),
		0, 0, 0, HEROBASEX+10, HEROBASEY+100);
	stream.str("");
	for(int i=1; i<=4; i++){ //每个魂的剩余点数
		stream << "剩余魂";
		stream << i;
		stream << ": ";
		stream << heroSet.heros[index].soul[i-1];
		m_label_soul[i-1] = new GUI::GUILabel("font\\simkai.ttf", 15, stream.str(),
			0, 0, 0, HEROBASEX+10, HEROBASEY+125+(i-1)*25);
		stream.str("");
	}
	m_listbox_heroMagic = new GUI::GUIListBox(HEROBASEX+10, HEROBASEY+230, 165, 7);
	//add magic to listbox
	for(int i=0; i<heroSet.heros[index].magicList.size(); i++){
		m_listbox_heroMagic->addItem(heroSet.heros[index].magicList[i]);
	}
}

MagicItem RPG_LearnMagic::getMagicItem(std::string magicName){
	for(int i=0; i<magicList.size(); i++){
		if(!strcmp(magicName.c_str(), magicList[i].magicName))
			return magicList[i];
	}
}

/*********************按钮事件处理****************************/
void RPG_LearnMagic::onBuy(){
	if(m_message!=NULL)return;
	if(m_listbox_learnMagic->getSelectItem()==-1)return;
	MagicItem t;
	t = getMagicItem(m_listbox_learnMagic->getSelectValue());
	if(heroPackage.finance < t.cost){ //金钱不够
		m_message = new GUI::GUIMessageBox("提示:", "金钱不足", "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
		return;
	}
	stringstream stream;
	int lose[4];
	for(int i=0; i<4; i++){
		lose[i] = heroSet.heros[m_currentHero].soul[i] - t.soul[i];
	}
	if( lose[0]<0 || lose[1]<0 || lose[2]<0 ||lose[3]<0){ //缺少至少一种魂
		stream << "缺少:\n\n";
		if(lose[0]<0){
			stream << "    ";
			stream << -lose[0];
			stream << " 点魂1\n";
		}
		if(lose[1]<0){
			stream << "    ";
			stream << -lose[1];
			stream << " 点魂2\n";
		}
		if(lose[2]<0){
			stream << "    ";
			stream << -lose[2];
			stream << " 点魂3\n";
		}
		if(lose[3]<0){
			stream << "    ";
			stream << -lose[3];
			stream << " 点魂4\n";
		}
		m_message = new GUI::GUIMessageBox("提示:", stream.str(), "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
		return;
	}
	int i;
	for(i=0; i<heroSet.heros[m_currentHero].magicList.size(); i++){
		if(!strcmp(t.magicName, heroSet.heros[m_currentHero].magicList[i].c_str())){//找到了
			break;
		}
	}
	if(i<heroSet.heros[m_currentHero].magicList.size()){//已经学过该魔法
		m_message = new GUI::GUIMessageBox("提示:", "不能重复学同一个魔法!", "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
		return;
	}
	//可以学习
	for(int i=0; i<4; i++){
		heroSet.heros[m_currentHero].soul[i] -= t.soul[i];
	}
	heroPackage.finance -= t.cost;
	m_message = new GUI::GUIMessageBox("提示:","你已经学会了 "+string(t.magicName), "image\\gui\\learnMagic\\learnMessageBoxBack.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxYes1.bmp", "image\\gui\\learnMagic\\learnMessageBoxYes2.bmp",
		"image\\gui\\learnMagic\\learnMessageBoxNo1.bmp", "image\\gui\\learnMagic\\learnMessageBoxNo2.bmp",
		200, 150, "font\\simkai.ttf");
	heroSet.heros[m_currentHero].magicList.push_back(string(t.magicName));
	//重刷GUI
	createHeroInterface(m_currentHero);
	delete m_label_money;
	stream.str("");
	stream << "$";
	stream << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 15, stream.str(),
		255, 255, 128, LEARNBASEX+45, LEARNBASEY+342);

}
void RPG_LearnMagic::onExit(){
	if(m_message!=NULL)return;
	status = LEARN_DEAD;
}
void RPG_LearnMagic::onLeft(){
	if(m_message!=NULL)return;
	if(m_currentHero==0)return;
	m_currentHero--;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onRight(){
	if(m_message!=NULL)return;
	if(m_currentHero>=heroSet.heroCount-1)return;
	m_currentHero++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd1(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[0]++;
	heroSet.heros[m_currentHero].totSoul[0]++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd2(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[1]++;
	heroSet.heros[m_currentHero].totSoul[1]++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd3(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[2]++;
	heroSet.heros[m_currentHero].totSoul[2]++;
	createHeroInterface(m_currentHero);
}
void RPG_LearnMagic::onAdd4(){
	if(m_message!=NULL)return;
	if(heroSet.heros[m_currentHero].pointSoul==0)return;
	heroSet.heros[m_currentHero].pointSoul--;
	heroSet.heros[m_currentHero].soul[3]++;
	heroSet.heros[m_currentHero].totSoul[3]++;
	createHeroInterface(m_currentHero);
}

///////////////////////////////////////////////////////////////
void RPG_LearnMagic::render(SDL_Surface* destSurf){
	if(status==LEARN_NULL)return;
	if(status==LEARN_DEAD){
		delete instance;
		status = LEARN_NULL;
		return;
	}
	//激活状态
	instance->m_back->drawImage(destSurf, LEARNBASEX, LEARNBASEY);
	/*************卖家界面****************/
	instance->m_button_exit->draw(destSurf);
	instance->m_button_buy->draw(destSurf);
	instance->m_listbox_learnMagic->draw(destSurf);
	instance->m_label_money->draw(destSurf);

	if(instance->m_listbox_learnMagic->getSelectItem()!=-1){//选中某个魔法
		if(instance->m_label_desc!=NULL){
			delete instance->m_label_desc;
			delete instance->m_label_cost;
			delete instance->m_label_needSoul;
		}
		MagicItem t;
		t = instance->getMagicItem(instance->m_listbox_learnMagic->getSelectValue());
		stringstream stream;
		string tempStr;

		stream << "描述：\n";
		tempStr = string(t.magicDesc);
		tempStr = newLine(tempStr, 12); //分行
		stream << tempStr;
		instance->m_label_desc = new GUI::GUILabel("font\\simkai.ttf", 16, stream.str(),
			0, 0, 0, LEARNBASEX+20, LEARNBASEY+165);

		stream.str("");
		stream << "价格：";
		stream << t.cost;
		instance->m_label_cost = new GUI::GUILabel("font\\simkai.ttf", 16, stream.str(),
			0, 0, 0, LEARNBASEX+20, LEARNBASEY+280);

		stream.str("");
		stream << "需要：";
		for(int i=0; i<4; i++){
			stream << t.soul[i];
			stream << "点魂";
			stream << i+1;
			stream << " ";
		}
		instance->m_label_needSoul = new GUI::GUILabel("font\\simkai.ttf", 12, stream.str(),
			0, 0, 0, LEARNBASEX+20, LEARNBASEY+300);
		
		instance->m_label_desc->draw(destSurf);
		instance->m_label_cost->draw(destSurf);
		instance->m_label_needSoul->draw(destSurf);
	}

	/****************英雄相关界面****************/
	instance->m_button_left->draw(destSurf);
	instance->m_button_right->draw(destSurf);
	instance->m_button_add1->draw(destSurf);
	instance->m_button_add2->draw(destSurf);
	instance->m_button_add3->draw(destSurf);
	instance->m_button_add4->draw(destSurf);

	instance->m_listbox_heroMagic->draw(destSurf);

	instance->m_label_zsh->draw(destSurf);
	instance->m_label_name->draw(destSurf);
	instance->m_label_totSoul->draw(destSurf);
	instance->m_label_pointSoul->draw(destSurf);
	for(int i=0; i<4; i++)instance->m_label_soul[i]->draw(destSurf);

	////////////////////////////////////////
	if(instance->m_message!=NULL){
		instance->m_message->draw(destSurf);
		if(instance->m_message->getSelection()!=GUI::GUIMessageBox::MB_NULL){//选择了
			delete instance->m_message;
			instance->m_message = NULL;
		}
	}

}