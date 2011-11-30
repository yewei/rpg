#include "../include/RPG_MenuSurface.h"
#include "../include/game.h"
#include "../include/RPG_DialogSystem.h"
#include "../include/RPG_TaskSystem.h"
#include "../include/RPG_EquipmentSystem.h"
#include <sstream>

bool RPG_NS_MenuSurface::RPG_MenuSurface::m_ableSurface;
int RPG_NS_MenuSurface::RPG_MenuSurface::m_numOfhero;
RPG_NS_MenuSurface::RPG_MenuSurface* RPG_NS_MenuSurface::RPG_MenuSurface::instance = NULL;
RPG_NS_Image::RPG_Image* RPG_NS_MenuSurface::RPG_MenuSurface::m_bk = NULL;
GUI::GUIButton* RPG_NS_MenuSurface::RPG_MenuSurface::m_hero1 = NULL;
GUI::GUIButton* RPG_NS_MenuSurface::RPG_MenuSurface::m_hero2 = NULL;
int RPG_NS_MenuSurface::RPG_MenuSurface::m_counter = 0;

RPG_NS_MenuSurface::RPG_MenuSurface::RPG_MenuSurface(void){
	m_status = NULL;
}

RPG_NS_MenuSurface::RPG_MenuSurface::~RPG_MenuSurface(void){
}

void RPG_NS_MenuSurface::RPG_MenuSurface::RPG_MenuSurface_Init(){

	setNumOfher(heroSet.heroCount);
	enableSurface(true);
	instance = new RPG_MenuSurface();
	//button_hero0
	std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[0].heroName)+"_s.bmp";
	instance->m_hero0 = new GUI::GUIButton(str, str, 30, 480, instance);
	instance->m_hero0->setfClickEvent(Hero0_Helper);
	instance->m_hero0->setfOverEvent(O_Hero0_Helper);
	instance->m_hero0->setfOutEvent(Ot_Hero0_Helper);
	//button_hero1
	if(m_numOfhero>=2){
		std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[1].heroName)+"_s.bmp";
	instance->m_hero1 = new GUI::GUIButton(str, str, 120, 480, instance);
		instance->m_hero1->setfClickEvent(Hero1_Helper);
		instance->m_hero1->setfOverEvent(O_Hero1_Helper);
		instance->m_hero1->setfOutEvent(Ot_Hero1_Helper);
	}
	else
		instance->m_hero1 = NULL;
	//button_hero2
	if(m_numOfhero>=3){
		std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[2].heroName)+"_s.bmp";
	instance->m_hero2 = new GUI::GUIButton(str, str, 209, 480, instance);
		instance->m_hero2->setfClickEvent(Hero2_Helper);
		instance->m_hero2->setfOverEvent(O_Hero2_Helper);
		instance->m_hero2->setfOutEvent(Ot_Hero2_Helper);
	}
	else
		instance->m_hero2 = NULL;
	//button_load
	instance->m_loaddlg = new GUI::GUIButton("image\\\gui\\\surface\\\menu_button_载入.bmp", 
		"image\\\gui\\\surface\\\menu_button_载入.bmp", 600, 510, instance);
	instance->m_loaddlg->setfClickEvent(Load_Helper);
	instance->m_loaddlg->setfOverEvent(O_Load_Helper);
	instance->m_loaddlg->setfOutEvent(Ot_Load_Helper);
	//button_save
	instance->m_savedlg = new GUI::GUIButton("image\\\gui\\\surface\\\menu_button_保存.bmp", 
		"image\\\gui\\\surface\\\menu_button_保存.bmp", 700, 510, instance);
	instance->m_savedlg->setfClickEvent(Save_Helper);
	instance->m_savedlg->setfOverEvent(O_Save_Helper);
	instance->m_savedlg->setfOutEvent(Ot_Save_Helper);
	//button_exit
	instance->m_exitdlg = new GUI::GUIButton("image\\\gui\\\surface\\\menu_button_退出.bmp", 
		"image\\\gui\\\surface\\\menu_button_退出.bmp", 600, 560, instance);
	instance->m_exitdlg->setfClickEvent(Exit_Helper);
	instance->m_exitdlg->setfOverEvent(O_Exit_Helper);
	instance->m_exitdlg->setfOutEvent(Ot_Exit_Helper);
	//button_task
	instance->m_taskdlg = new GUI::GUIButton("image\\\gui\\\surface\\\menu_button_任务.bmp", 
		"image\\\gui\\\surface\\\menu_button_任务.bmp", 600, 460, instance);
	instance->m_taskdlg->setfClickEvent(Task_Helper);
	instance->m_taskdlg->setfOverEvent(O_Task_Helper);
	instance->m_taskdlg->setfOutEvent(Ot_Task_Helper);
	//button_equipment
	instance->m_equipmentdlg = new GUI::GUIButton("image\\\gui\\\surface\\\menu_button_装备.bmp", 
		"image\\\gui\\\surface\\\menu_button_装备.bmp", 700, 460, instance);
	instance->m_equipmentdlg->setfClickEvent(Equipment_Helper);
	instance->m_equipmentdlg->setfOverEvent(O_Equipment_Helper);
	instance->m_equipmentdlg->setfOutEvent(Ot_Equipment_Helper);
	
	instance->m_monery = NULL;
	m_bk = new RPG_NS_Image::RPG_Image("image\\\gui\\\surface\\msurface.bmp");
}

void RPG_NS_MenuSurface::RPG_MenuSurface::render(SDL_Surface *pScreen){

	//button_hero1
	if(heroSet.heroCount>=2 && RPG_NS_MenuSurface::RPG_MenuSurface::m_hero1 == NULL){
		std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[1].heroName)+"_s.bmp";
	instance->m_hero1 = new GUI::GUIButton(str, str, 120, 480, instance);
		instance->m_hero1->setfClickEvent(Hero1_Helper);
		instance->m_hero1->setfOverEvent(O_Hero1_Helper);
		instance->m_hero1->setfOutEvent(Ot_Hero1_Helper);
	}
	//button_hero2
	if(heroSet.heroCount>=3 && RPG_NS_MenuSurface::RPG_MenuSurface::m_hero2 == NULL){
		std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[2].heroName)+"_s.bmp";
	instance->m_hero2 = new GUI::GUIButton(str, str, 220, 480, instance);
		instance->m_hero2->setfClickEvent(Hero1_Helper);
		instance->m_hero2->setfOverEvent(O_Hero1_Helper);
		instance->m_hero2->setfOutEvent(Ot_Hero1_Helper);
	}
	//
	m_bk->drawImage(pScreen, 0, 450);//画背景
	//画各按钮
	instance->m_hero0->draw(pScreen);
	if(instance->m_hero1 != NULL)
		instance->m_hero1->draw(pScreen);
	if(instance->m_hero2 != NULL)
		instance->m_hero2->draw(pScreen);
	instance->m_savedlg->draw(pScreen);
	instance->m_exitdlg->draw(pScreen);
	instance->m_loaddlg->draw(pScreen);
	instance->m_taskdlg->draw(pScreen);
	instance->m_equipmentdlg->draw(pScreen);
	//
	if(instance->m_monery != NULL){
		delete instance->m_monery;
		instance->m_monery = NULL;
	}
	std::string str;
	str = "$" + RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroPackage.finance) + "金";
	instance->m_monery = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT/2+2, str, 255, 255, 128, STUTAS_X, 575);
	instance->m_monery->draw(pScreen);

	if(instance->m_status != NULL)
		instance->m_status->draw(pScreen);
	//
	showHeroUpdate(pScreen);
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onHero0(){
	//弹出英雄一的具体信息
	if(m_counter)
		return;//正在显示其他英雄中
	std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[0].heroName)+".bmp";
	instance->m_heroP = new RPG_NS_Image::RPG_Image(str);
	createHeroshow(0);
	
}
void RPG_NS_MenuSurface::RPG_MenuSurface::overHero0(){
	
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "显示英雄一的具体信息\n名字，等级，经验值\n血量，魔法值，CP",
		128, 128, 32, STUTAS_X, STUTAS_Y);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outHero0(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onHero1(){
	//弹出英雄一的具体信息
	if(m_counter)
		return;//正在显示其他英雄中
	std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[1].heroName)+".bmp";
	instance->m_heroP = new RPG_NS_Image::RPG_Image(str);
	createHeroshow(1);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::overHero1(){
	
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "显示英雄二的具体信息\n名字，等级，经验值\n血量，魔法值，CP",
		128, 128, 32, STUTAS_X, STUTAS_Y);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outHero1(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onHero2(){
	
	//弹出英雄一的具体信息
	if(m_counter)
		return;//正在显示其他英雄中
	std::string str = "image\\gui\\surface\\hero\\hero_"+ std::string(heroSet.heros[2].heroName)+".bmp";
	instance->m_heroP = new RPG_NS_Image::RPG_Image(str);
	createHeroshow(2);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::overHero2(){
	
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "显示英雄三的具体信息\n名字，等级，经验值\n血量，魔法值，CP",
		128, 128, 32, STUTAS_X, STUTAS_Y);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outHero2(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onLoad(){
	
}
void RPG_NS_MenuSurface::RPG_MenuSurface::overLoad(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "载入游戏进度...",
		128, 128, 32, STUTAS_X, STUTAS_Y);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outLoad(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onSave(){
}
void RPG_NS_MenuSurface::RPG_MenuSurface::overSave(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "存储游戏进度...",
		128, 128, 32, STUTAS_X, STUTAS_Y);
	
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outSave(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onExit(){
	gameOver = true;
	
}
void RPG_NS_MenuSurface::RPG_MenuSurface::overExit(){
	
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "退出游戏",
		128, 128, 32, STUTAS_X, STUTAS_Y);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outExit(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onTask(){
	new RPG_NS_TaskSystem::RPG_TaskSystem(gameChapter-1);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::overTask(){
	
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "调出任务...",
		128, 128, 32, STUTAS_X, STUTAS_Y);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outTask(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}
//
void RPG_NS_MenuSurface::RPG_MenuSurface::onEquipment(){
	new RPG_NS_EquipmentSystem::RPG_EquipmentSystem();
}

void RPG_NS_MenuSurface::RPG_MenuSurface::overEquipment(){
	
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
	instance->m_status = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT, "打开装备：人物属性\n，人物背包，行囊背包",
		128, 128, 32, STUTAS_X, STUTAS_Y);
}
void RPG_NS_MenuSurface::RPG_MenuSurface::outEquipment(){
	if(instance->m_status != NULL){
		delete instance->m_status;
		instance->m_status = NULL;
	}
}

void RPG_NS_MenuSurface::RPG_MenuSurface::createHeroshow(int heroID){

	std::string t;
	instance->m_heroP->setColorKey(255, 0, 255);
	instance->m_heroShowB = new RPG_NS_Image::RPG_Image("image\\gui\\surface\\hero\\hero_status.bmp");
	instance->m_heroShowB->setColorKey(255, 0 , 255);
	//
	instance->m_hppb = new GUI::GUIProgressBar(68, 398, 118, 12, 128, 32, 12);
	instance->m_hppb->setRadio((double)(heroSet.heros[heroID].blood*1.0)/heroSet.heros[heroID].attribute.zdxl);
	instance->m_manapb = new GUI::GUIProgressBar(68, 425, 118, 12, 12, 32, 128);
	instance->m_manapb->setRadio((double)(heroSet.heros[heroID].mana*1.0)/heroSet.heros[heroID].attribute.zdml);
	instance->m_cppb = new GUI::GUIProgressBar(68, 452, 118, 12, 160, 160, 32);
	instance->m_cppb->setRadio((double)(heroSet.heros[heroID].cp*1.0)/100);
	//
	t = RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].blood) + "/" 
		+ RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].attribute.zdxl);
	instance->m_hp = new GUI::GUILabel("font\\simkai.ttf", 9, t, 255, 255, 255, 88, 400);
	t = RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].mana) + "/" 
		+ RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].attribute.zdml);
	instance->m_mana = new GUI::GUILabel("font\\simkai.ttf", 9, t, 255, 255, 255, 88, 427);
	t = RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].cp) + "/" 
		+ "100";
	instance->m_cp = new GUI::GUILabel("font\\simkai.ttf", 9, t, 255, 255, 255, 88, 454);
	//
	t = RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].level);
	instance->m_level = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT/2+2, t, 223, 219, 207, 177, 368);
	t = RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].experience) + "/"
		+ RPG_NS_DialogSystem::RPG_DialogSystem::intostring(heroSet.heros[heroID].nextExperience);
	instance->m_exp = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT/2+2, t, 128, 219, 207, 100, 472);
	instance->m_name = new GUI::GUILabel("font\\simkai.ttf", STUTAS_FONT/2+3, std::string(heroSet.heros[heroID].heroName), 219, 207, 168, 60, 367);

	//
	m_counter = 250;
	
}

void RPG_NS_MenuSurface::RPG_MenuSurface::showHeroUpdate(SDL_Surface *pScreen){
	
	if(!m_counter)
		return;
	if(m_counter == 1){
		delete instance->m_heroP;
		delete instance->m_heroShowB;
		delete instance->m_hp;
		delete instance->m_mana;
		delete instance->m_cp;
		delete instance->m_hppb;
		delete instance->m_manapb;
		delete instance->m_cppb;
		delete instance->m_name;
		delete instance->m_exp;
		delete instance->m_level;
	}
	if(m_counter >=200){//抽出过程	
		instance->m_heroP->drawImage(pScreen, -350 + (250-m_counter)*7, 0);
		instance->m_heroShowB->drawImage(pScreen, -350 + (250-m_counter)*7, 350);
	}
	else if(m_counter >= 50){//维持阶段
		instance->m_heroP->drawImage(pScreen, 0, 0);
		instance->m_heroShowB->drawImage(pScreen, 0, 350);
		instance->m_hppb->draw(pScreen);
		instance->m_manapb->draw(pScreen);
		instance->m_cppb->draw(pScreen);
		instance->m_hp->draw(pScreen);
		instance->m_mana->draw(pScreen);
		instance->m_cp->draw(pScreen);
		instance->m_name->draw(pScreen);
		instance->m_level->draw(pScreen);
		instance->m_exp->draw(pScreen);
	}
	else if(m_counter >1){//回收过程
		instance->m_heroP->drawImage(pScreen, (50-m_counter)*(-7), 0);
		instance->m_heroShowB->drawImage(pScreen, (50-m_counter)*(-7), 350);
	}
	m_counter--;
}