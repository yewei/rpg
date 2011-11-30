#include <sstream>
#include "../include/RPG_EquipmentSystem.h"
using RPG_NS_EquipmentSystem::RPG_EquipmentSystem;
using std::stringstream;

RPG_EquipmentSystem* RPG_EquipmentSystem::instance = NULL;
int RPG_EquipmentSystem::status = RPG_EquipmentSystem::EQP_NULL;

string RPG_EquipmentSystem::equipLabel[] = {
				"头盔:    ", "衣服:    ", "鞋子:    ", "武器:    ", "盾牌:    ", "头巾:    ", 
				"腰带:    ", "耳环1:    ", "耳环2:    ", "项链:    ","手镯1:    ", 
				"手镯2:    ", "戒指:    ", "护身符1:    ", "护身符2:    ", "护身符3:    "
};
string RPG_EquipmentSystem::statLabel[] = {
				"物理攻击:", "物理防御:", "魔法攻击:", "魔法防御:", "速度:"
};

RPG_EquipmentSystem::RPG_EquipmentSystem(){
	//读入文件,物品的详细描述信息
	////////////////////
	FILE* fp;
	fp = fopen("data\\item\\all.item", "rb");
	if(fp==NULL)throw string("无法打开文件")+string("data\\item\\all.item");
	Item item;
	while(!feof(fp)){
		int nRead = fread(&item, sizeof(Item), 1, fp);
		if(nRead==0)break;
		m_itemList.push_back(item);
	}
	fclose(fp);
	////////////////////
	setNULL();
	showHero(0); //默认显示第一个英雄

	//构造函数产生GUI中不变的部分
	m_image_back = new RPG_NS_Image::RPG_Image("image\\gui\\equip\\back.bmp");
	m_button_left = new GUI::GUIButton("image\\gui\\equip\\equipLeft1.bmp",
		"image\\gui\\equip\\equipLeft2.bmp", 40, 10, this);
	m_button_right = new GUI::GUIButton("image\\gui\\equip\\equipRight1.bmp",
		"image\\gui\\equip\\equipRight2.bmp", 200, 10, this);
	m_button_uninstall = new GUI::GUIButton("image\\gui\\equip\\equipUninstall1.bmp",
		"image\\gui\\equip\\equipUninstall2.bmp", 355, 420, this);
	m_button_install = new GUI::GUIButton("image\\gui\\equip\\equipInstall1.bmp",
		"image\\gui\\equip\\equipInstall2.bmp", 505, 420, this);
	m_button_drink = new GUI::GUIButton("image\\gui\\equip\\equipDrink1.bmp",
		"image\\gui\\equip\\equipDrink2.bmp", 655, 420, this);
	m_button_exit = new GUI::GUIButton("image\\gui\\equip\\equipExit1.bmp",
		"image\\gui\\equip\\equipExit2.bmp", 755, 20, this);
	status = EQP_LIVE; //激活
	instance = this;
	gamePause = true; //暂停游戏

	//添加事件
	m_button_left->setfClickEvent(onLeftHelper);
	m_button_right->setfClickEvent(onRightHelper);
	m_button_uninstall->setfClickEvent(onUninstallHelper);
	m_button_install->setfClickEvent(onInstallHelper);
	m_button_drink->setfClickEvent(onDrinkHelper);
	m_button_exit->setfClickEvent(onExitHelper);
}

RPG_EquipmentSystem::~RPG_EquipmentSystem(){
	delAllGUI();
	if(m_button_left!=NULL)delete m_button_left;
	if(m_button_right!=NULL)delete m_button_right;
	if(m_button_uninstall!=NULL)delete m_button_uninstall;
	if(m_button_install!=NULL)delete m_button_install;
	if(m_button_drink!=NULL)delete m_button_drink;
	if(m_button_exit!=NULL)delete m_button_exit;
}

void RPG_EquipmentSystem::showHero(int index){
	if(index<0 || index>=heroSet.heroCount)return;
	m_heroIndex = index;
	/******************删除过失的GUI控件*********************/
	delAllGUI();
	
	/********************************************************/

	/*******************添加新的GUI控件***********************/
	string heroName = string(heroSet.heros[index].heroName);
	m_image_hero = new RPG_NS_Image::RPG_Image("image\\gui\\surface\\hero\\hero_"+heroName+".bmp");
	m_image_hero->setColorKey(255, 0, 255);

	//左侧
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 21, string(heroSet.heros[index].heroName),
		0, 0, 0, 95, 17);
	m_label_level = new GUI::GUILabel("font\\simkai.ttf", 15, "等级: "+intToString(heroSet.heros[index].level),
		0, 0, 0, 30, 55);
	string exp = "经验: "+intToString(heroSet.heros[index].experience)
		+"  下一级经验: "+intToString(heroSet.heros[index].nextExperience);
	m_label_experience = new GUI::GUILabel("font\\simkai.ttf", 15, exp, 0, 0, 0, 30, 75);
	double blood_radio, mana_radio, cp_radio;
	blood_radio = (double)heroSet.heros[index].blood / heroSet.heros[index].attribute.zdxl; //血量百分比
	mana_radio = (double)heroSet.heros[index].mana / heroSet.heros[index].attribute.zdml; //魔量百分比
	cp_radio = (double)heroSet.heros[index].cp / 100.0; //CP百分比
	m_label_blood = new GUI::GUILabel("font\\simkai.ttf", 15, "血:"+intToString(heroSet.heros[index].blood)+
		"/"+intToString(heroSet.heros[index].attribute.zdxl), 0, 0, 0, 20, 445);
	m_label_mana = new GUI::GUILabel("font\\simkai.ttf", 15, "魔:"+intToString(heroSet.heros[index].mana)+
		"/"+intToString(heroSet.heros[index].attribute.zdml), 0, 0, 0, 20, 465);
	m_label_cp = new GUI::GUILabel("font\\simkai.ttf", 15, "CP:"+intToString(heroSet.heros[index].cp)+
		"/100", 0, 0, 0, 20, 485);
	m_progressbar_blood = new GUI::GUIProgressBar(115, 445, 130, 12, 250, 40, 60);//红色
	m_progressbar_blood->setRadio(blood_radio);
	m_progressbar_mana = new GUI::GUIProgressBar(115, 465, 130, 12, 70, 40, 240);//蓝色
	m_progressbar_mana->setRadio(mana_radio);
	m_progressbar_cp = new GUI::GUIProgressBar(115, 485, 130, 12, 220, 240, 60);//黄色
	m_progressbar_cp->setRadio(cp_radio);
	int basex, basey;
	basex = 20;
	basey = 515;
	int value[5];
	value[0] = heroSet.heros[index].attribute.wg;
	value[1] = heroSet.heros[index].attribute.wf;
	value[2] = heroSet.heros[index].attribute.mg;
	value[3] = heroSet.heros[index].attribute.mf;
	value[4] = heroSet.heros[index].attribute.sudu;
	int ext[5] = {0};
	for(int i=0; i<heroSet.items[m_heroIndex].size(); i++){
		Item t = getItemInfo(string(heroSet.items[m_heroIndex][i].material));
		ext[0] += t.eqs.wg;
		ext[1] += t.eqs.wf;
		ext[2] += t.eqs.mg;
		ext[3] += t.eqs.mf;
		ext[4] += t.eqs.sudu;
	}
	for(int i=0; i<5; i++){//人物属性
		string str = statLabel[i]+intToString(value[i]);
		if(ext[i]>0) str += " +"+intToString(ext[i]);
		else if(ext[i]<0) str += " -"+intToString(ext[i]);
		m_label_attribute[i] = new GUI::GUILabel("font\\simkai.ttf", 15, str,
			0, 0, 0, basex+(i%2)*120, basey+(i/2)*20);
	}	
	//右侧
	for(int i=0; i<heroSet.items[index].size(); i++){
		//枚举英雄身上的装备显示到合适位置
		Item t = getItemInfo(string(heroSet.items[index][i].material));
		if(t.type!=0 && t.type!=2)continue; //只有装备物品才能装
		if(m_hasEquip[t.eqpID]==""){
			m_hasEquip[t.eqpID] = string(t.material);//添加装备名称
		}
		else{//察看可以配戴多个的情况
			if(t.eqpID==EQP_ERHUAN && m_hasEquip[EQP_ERHUAN+1]==""){//耳环两个
				m_hasEquip[EQP_ERHUAN+1] = string(t.material);
			}
			else if(t.eqpID==EQP_SHOUZHUO && m_hasEquip[EQP_SHOUZHUO+1]==""){//手镯两个
				m_hasEquip[EQP_SHOUZHUO+1] = string(t.material);
			}
			else if(t.eqpID==EQP_HUSHENFU){//护身符
				if(m_hasEquip[EQP_HUSHENFU+1]==""){
					m_hasEquip[EQP_HUSHENFU+1] = string(t.material);
				}
				else if(m_hasEquip[EQP_HUSHENFU+2]==""){
					m_hasEquip[EQP_HUSHENFU+2] = string(t.material);
				}
			}
		}
	}
	basex = 370;
	basey = 50;
	for(int i=0; i<16; i++){
		string str = equipLabel[i]+m_hasEquip[i];
		m_label_equipLabel[i] = new GUI::GUILabel("font\\simkai.ttf", 16, str,
			0, 0, 0, basex+(i/8)*250, basey+(i%8)*20); //身上装备信息
	}

	m_listbox_hequip = new GUI::GUIListBox(340, 240, 100, 10);
	m_listbox_pequip = new GUI::GUIListBox(490, 240, 100, 10);
	m_listbox_drug = new GUI::GUIListBox(640, 240, 100, 10);
	//add to listbox
	for(int i=0; i<heroSet.items[index].size(); i++)
		m_listbox_hequip->addItem(string(heroSet.items[index][i].material));
	for(int i=0; i<heroPackage.weapon.size(); i++){
		string str = string(heroPackage.weapon[i].material)
			+"("+intToString(heroPackage.weapon[i].num)+")";
		m_listbox_pequip->addItem(str);
	}
	for(int i=0; i<heroPackage.jewelry.size(); i++){
		string str = string(heroPackage.jewelry[i].material)
			+"("+intToString(heroPackage.jewelry[i].num)+")";
		m_listbox_pequip->addItem(str);
	}
	for(int i=0; i<heroPackage.story.size(); i++){
		string str = string(heroPackage.story[i].material)
			+"("+intToString(heroPackage.story[i].num)+")";
		m_listbox_pequip->addItem(str);
	}
	for(int i=0; i<heroPackage.drug.size(); i++){
		string str = string(heroPackage.drug[i].material)
			+"("+intToString(heroPackage.drug[i].num)+")";
		m_listbox_drug->addItem(str);
	}
	
	/********************************************************/
}

void RPG_EquipmentSystem::render(SDL_Surface* destSurf){
	if(status==EQP_NULL)return;
	if(status==EQP_DEAD){
		delete instance;
		instance = NULL;
		return;
	}
	//图片
	instance->m_image_back->drawImage(destSurf, 0, 0); //背景图片
	instance->m_image_hero->drawImage(destSurf, 25, 90); //英雄的图片

	//按钮
	instance->m_button_left->draw(destSurf); //向左按钮
	instance->m_button_right->draw(destSurf); //向右按钮
	instance->m_button_uninstall->draw(destSurf); //卸载装备
	instance->m_button_install->draw(destSurf); //装备上去
	instance->m_button_drink->draw(destSurf); //喝药
	instance->m_button_exit->draw(destSurf); //回到游戏
	
	//进度条
	instance->m_progressbar_blood->draw(destSurf); //血
	instance->m_progressbar_mana->draw(destSurf); //魔
	instance->m_progressbar_cp->draw(destSurf); //CP
	//列表框
	instance->m_listbox_hequip->draw(destSurf); //英雄身上的装备列表
	instance->m_listbox_pequip->draw(destSurf); //包裹中的装备列表
	instance->m_listbox_drug->draw(destSurf); //包裹中的药品列表
	//标签框
	/**************************************/
	instance->m_label_name->draw(destSurf); //人物名称
	instance->m_label_level->draw(destSurf); //人物等级
	instance->m_label_experience->draw(destSurf); //经验,下一级经验
	instance->m_label_blood->draw(destSurf); //血,百分比
	instance->m_label_mana->draw(destSurf); //魔,百分比
	instance->m_label_cp->draw(destSurf); //CP,百分比
	for(int i=0; i<5; i++)
		instance->m_label_attribute[i]->draw(destSurf); //物理攻击,物理防御,魔法攻击,魔法防御,速度信息
	/**************************************/
	for(int i=0; i<16; i++)
		instance->m_label_equipLabel[i]->draw(destSurf);//装备的种类标签
	if(instance->m_listbox_hequip->getSelectItem()>=0){//"卸载列表框"有选择
		if(instance->desc1!=NULL)delete instance->desc1;
		string value = instance->m_listbox_hequip->getSelectValue();
		Item t = instance->getItemInfo(value);
		string str = newLine(t.desc, 6);
		instance->desc1 = new GUI::GUILabel("font\\simkai.ttf", 15, str, 0, 0, 0, 340, 460);
		instance->desc1->draw(destSurf);
	}
	if(instance->m_listbox_pequip->getSelectItem()>=0){//"装备列表框"有选择
		if(instance->desc2!=NULL)delete instance->desc2;
		string value = instance->m_listbox_pequip->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = instance->getItemInfo(value);
		string str = newLine(t.desc, 6);
		instance->desc2 = new GUI::GUILabel("font\\simkai.ttf", 15, str, 0, 0, 0, 490, 460);
		instance->desc2->draw(destSurf);
	}
	if(instance->m_listbox_drug->getSelectItem()>=0){//"食用列表框"有选择
		if(instance->desc3!=NULL)delete instance->desc3;
		string value = instance->m_listbox_drug->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = instance->getItemInfo(value);
		string str = newLine(t.desc, 6);
		instance->desc3 = new GUI::GUILabel("font\\simkai.ttf", 15, str, 0, 0, 0, 640, 460);
		instance->desc3->draw(destSurf);
	}
}

string RPG_EquipmentSystem::intToString(int n){
	stringstream stream;
	stream << n;
	return stream.str();
}

Item RPG_EquipmentSystem::getItemInfo(string material){
	for(int i=0; i<m_itemList.size(); i++){
		if(strcmp(m_itemList[i].material, material.c_str())==0){
			//找到该物品
			return m_itemList[i];
		}
	}
	return Item();
}

void RPG_EquipmentSystem::setNULL(){
	m_image_hero = NULL;

	m_progressbar_blood = NULL;
	m_progressbar_mana = NULL;
	m_progressbar_cp = NULL;

	m_listbox_hequip = NULL;
	m_listbox_pequip = NULL;
	m_listbox_drug = NULL;

	m_label_name = NULL;
	m_label_level = NULL;
	m_label_experience = NULL;
	m_label_blood = NULL;
	m_label_mana = NULL;
	m_label_cp = NULL;
	desc1 = desc2 = desc3 = NULL;
	for(int i=0; i<5; i++){
		m_label_attribute[i] = NULL;
	}
	for(int i=0; i<16; i++){
		m_label_equipLabel[i] = NULL;
	}
}

void RPG_EquipmentSystem::delAllGUI(){
	if(m_image_hero!=NULL)delete m_image_hero;

	if(m_progressbar_blood!=NULL)delete m_progressbar_blood;
	if(m_progressbar_mana!=NULL)delete m_progressbar_mana;
	if(m_progressbar_cp!=NULL)delete m_progressbar_cp;

	if(m_listbox_hequip!=NULL)delete m_listbox_hequip;
	if(m_listbox_pequip!=NULL)delete m_listbox_pequip;
	if(m_listbox_drug!=NULL)delete m_listbox_drug;

	if(m_label_name!=NULL)delete m_label_name;
	if(m_label_level!=NULL)delete m_label_level;
	if(m_label_experience!=NULL)delete m_label_experience;
	if(m_label_blood!=NULL)delete m_label_blood;
	if(m_label_mana!=NULL)delete m_label_mana;
	if(m_label_cp!=NULL)delete m_label_cp;
	for(int i=0; i<5; i++){
		if(m_label_attribute[i]!=NULL)delete m_label_attribute[i];
	}
	for(int i=0; i<16; i++){
		if(m_label_equipLabel[i]!=NULL)delete m_label_equipLabel[i];
		m_hasEquip[i] = "";
	}
	setNULL();
}

/****************************按钮处理**********************************/
void RPG_EquipmentSystem::onLeft(){
	if(m_heroIndex==0)return;
	m_heroIndex--;
	showHero(m_heroIndex);
}

void RPG_EquipmentSystem::onRight(){
	if(m_heroIndex==heroSet.heroCount-1)return;
	m_heroIndex++;
	showHero(m_heroIndex);
}

void RPG_EquipmentSystem::onUninstall(){
	if(m_listbox_hequip->getSelectItem()>=0){//"卸载列表框"有选择
		string value = m_listbox_hequip->getSelectValue();
		Item t = getItemInfo(value);
	
		//从英雄身上卸下装备
		delFromHero(m_heroIndex, value);
		//添加到包裹中
		addToPackage(t.type, value);
		/////更新英雄的参数//////

		///////////////////////
		showHero(m_heroIndex);
	}
}

void RPG_EquipmentSystem::onInstall(){
	if(m_listbox_pequip->getSelectItem()>=0){//"装备列表框"有选择
		string value = m_listbox_pequip->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = getItemInfo(value);
		if(t.type!=0 && t.type!=2)return;; //只有装备物品才能装
		bool bFixed = false; //是否有空位装
		if(m_hasEquip[t.eqpID]==""){
			//m_hasEquip[t.eqpID] = string(t.material);//添加装备名称
			bFixed = true;
		}
		else{//察看可以配戴多个的情况
			if(t.eqpID==EQP_ERHUAN && m_hasEquip[EQP_ERHUAN+1]==""){//耳环两个
				//m_hasEquip[EQP_ERHUAN+1] = string(t.material);
				bFixed = true;
			}
			else if(t.eqpID==EQP_SHOUZHUO && m_hasEquip[EQP_SHOUZHUO+1]==""){//手镯两个
				//m_hasEquip[EQP_SHOUZHUO+1] = string(t.material);
				bFixed = true;
			}
			else if(t.eqpID==EQP_HUSHENFU){//护身符
				if(m_hasEquip[EQP_HUSHENFU+1]==""){
					//m_hasEquip[EQP_HUSHENFU+1] = string(t.material);
					bFixed = true;
				}
				else if(m_hasEquip[EQP_HUSHENFU+2]==""){
					//m_hasEquip[EQP_HUSHENFU+2] = string(t.material);
					bFixed = true;
				}
			}
		}
		if(bFixed){//可以装备
			//添加到英雄身上
			addToHero(m_heroIndex, value);
			//从包裹中删除一个
			delFromPackage(t.type, value);
			/////更新英雄的参数//////

			///////////////////////
			showHero(m_heroIndex);

		}
	}
}

void RPG_EquipmentSystem::onDrink(){
	if(instance->m_listbox_drug->getSelectItem()>=0){//"食用列表框"有选择
		string value = instance->m_listbox_drug->getSelectValue();
		value = value.substr(0, value.size()-3);
		Item t = instance->getItemInfo(value);
		if(t.dgs.canUse==false)return; //该药品不能在装备系统中使用
		int add;
		//增加英雄的血,魔,或CP
		add = t.dgs.addBlood;
		if(add>=RADIO_BASE){//血按百分比加
			add = heroSet.heros[m_heroIndex].attribute.zdxl * ((add-RADIO_BASE)/100.0);
		}
		heroSet.heros[m_heroIndex].blood += add;
		if(heroSet.heros[m_heroIndex].blood>heroSet.heros[m_heroIndex].attribute.zdxl){
			heroSet.heros[m_heroIndex].blood = heroSet.heros[m_heroIndex].attribute.zdxl;//不能超过最大血量
		}

		add = t.dgs.addMana;
		if(add>=RADIO_BASE){//魔按百分比加
			add = heroSet.heros[m_heroIndex].attribute.zdml * ((add-RADIO_BASE)/100.0);
		}
		heroSet.heros[m_heroIndex].mana += add;
		if(heroSet.heros[m_heroIndex].mana>heroSet.heros[m_heroIndex].attribute.zdml){
			heroSet.heros[m_heroIndex].mana = heroSet.heros[m_heroIndex].attribute.zdml;//不能超过最大魔量
		}

		add = t.dgs.addCp;
		if(add>=RADIO_BASE){//CP按百分比加
			add = 100 * ((add-RADIO_BASE)/100.0);
		}
		heroSet.heros[m_heroIndex].cp += add;
		if(heroSet.heros[m_heroIndex].cp>100){
			heroSet.heros[m_heroIndex].cp = 100;//不能超过最大cp量
		}
		//删除一个该药品
		delFromPackage(1, value);
		showHero(m_heroIndex);
	}
}

void RPG_EquipmentSystem::onExit(){
	status = EQP_DEAD;
	gamePause = false;
}