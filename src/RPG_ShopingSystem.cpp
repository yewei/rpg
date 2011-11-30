#include <string.h>
#include <stdio.h>
#include <sstream>
#include "../include/RPG_ShopingSystem.h"
#include "../include/RPG_DialogSystem.h"
#include "../include/game.h"
using RPG_NS_ShopingSystem::RPG_ShopingSystem;
using RPG_NS_DialogSystem::RPG_DialogSystem;
using std::stringstream;
using GUI::GUIMessageBox;

int RPG_NS_ShopingSystem::RPG_ShopingSystem::status = RPG_NS_ShopingSystem::RPG_ShopingSystem::SHOP_NULL;
RPG_NS_ShopingSystem::RPG_ShopingSystem* RPG_NS_ShopingSystem::RPG_ShopingSystem::instance = NULL;

RPG_NS_ShopingSystem::RPG_ShopingSystem::RPG_ShopingSystem(int mapID, int unicode){
	string fileName;
	stringstream stream;
	stream << mapID;
	stream << "U";
	stream << unicode;
	m_mapID = mapID;
	m_npcUnicode = unicode;
	//从fileName文件中读取所卖物品列表
	fileName = "data\\shop\\mapID"+stream.str()+".shop";
	FILE* fp = fopen(fileName.c_str(), "r");
	if(fp==NULL)throw "无法打开文件"+fileName;
	char material[MAX_NAME];
	int k = 0;
	while(!feof(fp)){
		fscanf(fp, "%s", material);
		if(strcmp(material, "END")==0)break;
		if(strcmp(material, "~")==0){k++;continue;}
		string name(material);
		if(k==0)m_weaponList.push_back(name);
		if(k==1)m_drugList.push_back(name);
		if(k==2)m_jewelryList.push_back(name);
	}
	fclose(fp);
	//从data\item\all.item中读取所有物品信息
	fp = fopen("data\\item\\all.item", "rb");
	if(fp==NULL)throw string("无法打开文件")+string("data\\item\\all.item");
	Item item;
	while(!feof(fp)){
		int nRead = fread(&item, sizeof(Item), 1, fp);
		if(nRead==0)break;
		m_itemList.push_back(item);
	}
	fclose(fp);
	instance = this; //添加实例
	status = SHOP_LIVE; //激活
	m_interface = SHOP_TOP; //默认显示顶层按钮
	gamePause = true; //暂停游戏

	//
	m_button_top_buy = new GUI::GUIButton("image\\gui\\shop\\shopTopBuy1.bmp", "image\\gui\\shop\\shopTopBuy2.bmp",
		SHOP_STARTX+60, SHOP_STARTY+50, this);
	m_button_top_sale = new GUI::GUIButton("image\\gui\\shop\\shopTopSale1.bmp", "image\\gui\\shop\\shopTopBuy2.bmp",
		SHOP_STARTX+60, SHOP_STARTY+85, this);
	m_button_top_say = new GUI::GUIButton("image\\gui\\shop\\shopTopSay1.bmp", "image\\gui\\shop\\shopTopSay2.bmp",
		SHOP_STARTX+60, SHOP_STARTY+120, this);
	m_button_top_buy->setfClickEvent(TopBuyHelper);
	m_button_top_sale->setfClickEvent(TopSaleHelper);
	m_button_top_say->setfClickEvent(TopSayHelper);
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::render(SDL_Surface* destSurf){ //static
	if(status==SHOP_NULL)return;//无需执行
	if(status==SHOP_DEAD){ //删除已经死亡的shop
		delete instance;
		instance = NULL;
		status = SHOP_NULL;
		return;
	}
	//激活状态
	if(instance->m_interface==SHOP_TOP){
		instance->m_button_top_buy->draw(destSurf);
		instance->m_button_top_sale->draw(destSurf);
		instance->m_button_top_say->draw(destSurf);
	}else if(instance->m_interface==SHOP_BUY||instance->m_interface==SHOP_SALE){
		instance->m_back->drawImage(destSurf, SHOP_STARTX, SHOP_STARTY);
		instance->m_button_left->draw(destSurf);
		instance->m_button_right->draw(destSurf);
		instance->m_label_name->draw(destSurf);
		instance->m_listbox_items->draw(destSurf);
		instance->m_button_ack->draw(destSurf);
		instance->m_button_exit->draw(destSurf);
		instance->m_label_money->draw(destSurf);
		if(instance->m_listbox_items->getSelectItem()!=-1){ //选中某个物品
			if(instance->m_label_desc!=NULL)delete instance->m_label_desc;
			if(instance->m_label_cost!=NULL)delete instance->m_label_cost;
			string name = instance->m_listbox_items->getSelectValue(); //物品名称
			string desc = "";
			int cost = 0;
			for(int i=0; i<instance->m_itemList.size(); i++){
				if(strcmp(instance->m_itemList[i].material, name.c_str())==0){
					desc = string(instance->m_itemList[i].desc);
					cost = instance->m_itemList[i].cost;
					break;
				}
			}
			if(instance->m_interface==SHOP_SALE)cost = cost*0.3; //以30%卖掉物品
			stringstream stream;
			stream << cost;
			string strCost = "价格:"+stream.str();
			//desc需要适当换行(20字左右换)
			desc = newLine(desc, 10);
			instance->m_label_desc = new GUI::GUILabel("font\\simkai.ttf", 
				15, desc, 0,0,0, SHOP_STARTX+35, SHOP_STARTY+185);
			instance->m_label_cost = new GUI::GUILabel("font\\simkai.ttf",
				15, strCost, 0,0,0, SHOP_STARTX+35, SHOP_STARTY+315);
			instance->m_label_desc->draw(destSurf);
			instance->m_label_cost->draw(destSurf);
			
			int nCount = 0; //拥有的数量
			if(instance->m_interface==SHOP_SALE){
				if(instance->m_label_count!=NULL)delete instance->m_label_count;
				//正在卖东西,显示拥有的数量
				if(instance->m_wdj==0){//武器
					for(int i=0; i<heroPackage.weapon.size(); i++){
						if(strcmp(heroPackage.weapon[i].material, name.c_str())==0){
							nCount = heroPackage.weapon[i].num;
						}
					}
				}
				else if(instance->m_wdj==1){//药品
					for(int i=0; i<heroPackage.drug.size(); i++){
						if(strcmp(heroPackage.drug[i].material, name.c_str())==0){
							nCount = heroPackage.drug[i].num;
						}
					}
				}else{
					for(int i=0; i<heroPackage.jewelry.size(); i++){
						if(strcmp(heroPackage.jewelry[i].material, name.c_str())==0){
							nCount = heroPackage.jewelry[i].num;
						}
					}
				}
				stringstream strCount;
				strCount << nCount;
				string lcount = "数量:"+strCount.str();
				instance->m_label_count = new GUI::GUILabel("font\\simkai.ttf",
				15, lcount, 0,0,0, SHOP_STARTX+35, SHOP_STARTY+300);
				instance->m_label_count->draw(destSurf);
			}
		}
		if(instance->m_messagebox!=NULL){//出现信息框
			if(instance->m_messagebox->getSelection()==GUIMessageBox::MB_NULL){//玩家还没有做选择
				instance->m_messagebox->draw(destSurf);
			}else{//玩家选择了
				if(instance->m_doExchanging){ //交易期
					if(instance->m_messagebox->getSelection()==GUIMessageBox::MB_YES){//玩家确认
						delete instance->m_messagebox;
						instance->m_messagebox = NULL;
						if(instance->m_interface==SHOP_BUY)
							instance->doBuying();
						else if(instance->m_interface==SHOP_SALE)
							instance->doSaling();
					}
					else{//玩家取消
						instance->m_doExchanging = false; //取消交易
						delete instance->m_messagebox;
						instance->m_messagebox = NULL;
					}
				}
				else{ //提示,非交易
					delete instance->m_messagebox;
					instance->m_messagebox = NULL;
				}
			}
		}
	}else if(instance->m_interface==SHOP_SAY){
	
	}
	
}

/****************************所有按钮事件的处理*****************************/
void RPG_NS_ShopingSystem::RPG_ShopingSystem::initInterface(int interfaceID){
	m_interface = interfaceID; //选择界面
	//删除顶层按钮
	delete m_button_top_buy;
	m_button_top_buy = NULL;
	delete m_button_top_sale;
	m_button_top_sale = NULL;
	delete m_button_top_say;
	m_button_top_say = NULL;

	m_wdj = 0; //默认买卖"武器"
	m_doExchanging = false;

	//初始化"买卖东西"界面
	m_back = new RPG_NS_Image::RPG_Image("image\\gui\\shop\\shopBack.bmp");
	m_button_left = new GUI::GUIButton("image\\gui\\shop\\shopLeft1.bmp", "image\\gui\\shop\\shopLeft2.bmp",
		SHOP_STARTX+35, SHOP_STARTY+5, this);
	m_button_right = new GUI::GUIButton("image\\gui\\shop\\shopRight1.bmp", "image\\gui\\shop\\shopRight2.bmp",
		SHOP_STARTX+183, SHOP_STARTY+5, this);
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 20, "武器",0,0,0, SHOP_STARTX+100, SHOP_STARTY+15);
	m_listbox_items = new GUI::GUIListBox(SHOP_STARTX+50, SHOP_STARTY+60, 150, 7);
	if(interfaceID==SHOP_BUY){//"买"
		m_button_ack = new GUI::GUIButton("image\\gui\\shop\\shopAckBuy1.bmp", "image\\gui\\shop\\shopAckBuy2.bmp",
		SHOP_STARTX+180, SHOP_STARTY+340, this);
	}
	else{//"卖"
		m_button_ack = new GUI::GUIButton("image\\gui\\shop\\shopAckSale1.bmp", "image\\gui\\shop\\shopAckSale2.bmp",
		SHOP_STARTX+180, SHOP_STARTY+340, this);
	}
	m_button_exit = new GUI::GUIButton("image\\gui\\shop\\shopExit1.bmp", "image\\gui\\shop\\shopExit2.bmp", 
		SHOP_STARTX+223, SHOP_STARTY+12, this);
	stringstream total;
	total << "$";
	total << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 16, total.str(),
		255, 255, 128, SHOP_STARTX+45, SHOP_STARTY+340);
	m_label_desc = NULL;
	m_label_cost = NULL;
	m_label_count = NULL;
	m_messagebox = NULL;
	//添加事件
	m_button_left->setfClickEvent(LeftHelper);
	m_button_right->setfClickEvent(RightHelper);
	m_button_ack->setfClickEvent(AckHelper);
	m_button_exit->setfClickEvent(ExitHelper);
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onTopBuy(){
	initInterface(SHOP_BUY);
	//添加"武器"物品
	for(int i=0; i<m_weaponList.size(); i++){
		m_listbox_items->addItem(m_weaponList[i]);
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onTopSale(){
	initInterface(SHOP_SALE);
	for(int i=0; i<heroPackage.weapon.size(); i++){//卖东西时，添加人物包裹中的武器
		m_listbox_items->addItem(string(heroPackage.weapon[i].material));
	}

}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onTopSay(){
	delete m_button_top_buy;
	m_button_top_buy = NULL;
	delete m_button_top_sale;
	m_button_top_sale = NULL;
	delete m_button_top_say;
	m_button_top_say = NULL;
	status = SHOP_DEAD; //消除ShopSystem
	gamePause = false;

	RPG_DialogSystem::createDialog(m_mapID, m_npcUnicode);//调出对话系统

}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onLeft(){ //切换"武器","药品","首饰"
	if(instance->m_wdj==0)return;
	//有信息框,没确认则不响应其他消息
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	instance->m_wdj--;
	//删除过时的控件
	delete instance->m_label_name;
	delete instance->m_listbox_items;
	if(instance->m_label_desc!=NULL){
		delete instance->m_label_desc;
		instance->m_label_desc = NULL;
	}
	if(instance->m_label_cost!=NULL){
		delete instance->m_label_cost;
		instance->m_label_cost = NULL;
	}
	if(instance->m_label_count!=NULL){
		delete instance->m_label_count;
		instance->m_label_count = NULL;
	}
	//添加组件
	string name;
	if(m_wdj==0)name = "武器";
	else if(m_wdj==1)name = "药品";
	else if(m_wdj==2)name = "首饰";
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 20, name,0,0,0, SHOP_STARTX+100, SHOP_STARTY+15);
	m_listbox_items = new GUI::GUIListBox(SHOP_STARTX+50, SHOP_STARTY+60, 150, 7);
	//根据是“买”还是“卖”添加相应物品
	if(m_interface==SHOP_BUY){ //“买”，添加从文件里读入的物品
		if(m_wdj==0){
			for(int i=0; i<m_weaponList.size(); i++){
				m_listbox_items->addItem(m_weaponList[i]);
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<m_drugList.size(); i++){
				m_listbox_items->addItem(m_drugList[i]);
			}
		}
		else{
			for(int i=0; i<m_jewelryList.size(); i++){
				m_listbox_items->addItem(m_jewelryList[i]);
			}
		}
	}
	else{//“卖”，添加人物包裹中的物品
		if(m_wdj==0){
			for(int i=0; i<heroPackage.weapon.size(); i++){//卖东西时，添加人物包裹中的武器
				m_listbox_items->addItem(string(heroPackage.weapon[i].material));
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<heroPackage.drug.size(); i++){
				m_listbox_items->addItem(string(heroPackage.drug[i].material));
			}
		}
		else{
			for(int i=0; i<heroPackage.jewelry.size(); i++){
				m_listbox_items->addItem(string(heroPackage.jewelry[i].material));
			}
		}
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onRight(){
	if(instance->m_wdj==2)return;
	//有信息框,没确认则不响应其他消息
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	instance->m_wdj++;
	//删除过时的控件
	delete instance->m_label_name;
	delete instance->m_listbox_items;
	if(instance->m_label_desc!=NULL){
		delete instance->m_label_desc;
		instance->m_label_desc = NULL;
	}
	if(instance->m_label_cost!=NULL){
		delete instance->m_label_cost;
		instance->m_label_cost = NULL;
	}
	if(instance->m_label_count!=NULL){
		delete instance->m_label_count;
		instance->m_label_count = NULL;
	}
	//添加组件
	string name;
	if(m_wdj==0)name = "武器";
	else if(m_wdj==1)name = "药品";
	else if(m_wdj==2)name = "首饰";
	m_label_name = new GUI::GUILabel("font\\simkai.ttf", 20, name,0,0,0, SHOP_STARTX+100, SHOP_STARTY+15);
	m_listbox_items = new GUI::GUIListBox(SHOP_STARTX+50, SHOP_STARTY+60, 150, 7);
	//根据是“买”还是“卖”添加相应物品
	if(m_interface==SHOP_BUY){ //“买”，添加从文件里读入的物品
		if(m_wdj==0){
			for(int i=0; i<m_weaponList.size(); i++){
				m_listbox_items->addItem(m_weaponList[i]);
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<m_drugList.size(); i++){
				m_listbox_items->addItem(m_drugList[i]);
			}
		}
		else{
			for(int i=0; i<m_jewelryList.size(); i++){
				m_listbox_items->addItem(m_jewelryList[i]);
			}
		}
	}
	else{//“卖”，添加人物包裹中的物品
		if(m_wdj==0){
			for(int i=0; i<heroPackage.weapon.size(); i++){//卖东西时，添加人物包裹中的武器
				m_listbox_items->addItem(string(heroPackage.weapon[i].material));
			}
		}
		else if(m_wdj==1){
			for(int i=0; i<heroPackage.drug.size(); i++){
				m_listbox_items->addItem(string(heroPackage.drug[i].material));
			}
		}
		else{
			for(int i=0; i<heroPackage.jewelry.size(); i++){
				m_listbox_items->addItem(string(heroPackage.jewelry[i].material));
			}
		}
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onAck(){
	//有信息框,没确认则不响应其他消息
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	if(m_listbox_items->getSelectItem()<0){ //还没选中物品
		if(m_messagebox!=NULL)delete m_messagebox;
		m_messagebox = new GUI::GUIMessageBox("", "没有选择物品!", "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		return;
	}
	
	if(m_interface==SHOP_BUY){//买物品
		string msg = "确实要买 "+m_listbox_items->getSelectValue()+" 吗?";
		msg = newLine(msg, 10);
		m_messagebox = new GUI::GUIMessageBox("", msg, "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		m_doExchanging = true; //进入交易期
		return;
	}
	if(m_interface==SHOP_SALE){//卖物品
		string msg = "确实要卖 "+m_listbox_items->getSelectValue()+" 吗?";
		msg = newLine(msg, 10);
		m_messagebox = new GUI::GUIMessageBox("", msg, "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		m_doExchanging = true; //进入交易期
		return;
	}
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::onExit(){
	//有信息框,没确认则不响应其他消息
	if(m_messagebox!=NULL && m_messagebox->getSelection()==GUIMessageBox::MB_NULL)return;
	delete m_back;
	delete m_button_left;
	delete m_button_right;
	delete m_label_name;
	delete m_listbox_items;
	if(m_label_desc!=NULL)delete m_label_desc;
	if(m_label_count!=NULL)delete m_label_count;
	delete m_button_ack;
	delete m_button_exit;
	delete m_label_money;
	status = SHOP_DEAD; //消除ShopSystem
	gamePause = false;
}

/****************************************************************************/
void RPG_NS_ShopingSystem::RPG_ShopingSystem::doBuying(){
	int cost = 0;
	string name = m_listbox_items->getSelectValue();
	for(int i=0; i<m_itemList.size(); i++){
		if(strcmp(m_itemList[i].material, name.c_str())==0){
			cost = m_itemList[i].cost;
			break;
		}
	}
	if(cost>heroPackage.finance){
		m_messagebox = new GUI::GUIMessageBox("", "资金不足!", "image\\gui\\shop\\shopMessageBoxBack.bmp",
			"image\\gui\\shop\\shopMessageBoxYes1.bmp", "image\\gui\\shop\\shopMessageBoxYes2.bmp", 
			"image\\gui\\shop\\shopMessageBoxNo1.bmp", "image\\gui\\shop\\shopMessageBoxNo2.bmp",
			SHOP_STARTX-60, SHOP_STARTY+20, "font\\simkai.ttf");
		m_doExchanging = false;//取消交易
		return;
	}

	heroPackage.finance -= cost;
	addToPackage(m_wdj, name.c_str()); //添加进包裹
	m_doExchanging = false;
	if(m_wdj==0){ //为了更新列表信息
		onRight();
		onLeft();
	}else{
		onLeft();
		onRight();
	}
	delete m_label_money;
	stringstream total;
	total << "$";
	total << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 16, total.str(),
		255, 255, 128, SHOP_STARTX+45, SHOP_STARTY+340);
}

void RPG_NS_ShopingSystem::RPG_ShopingSystem::doSaling(){
	int cost = 0;
	string name = m_listbox_items->getSelectValue();
	for(int i=0; i<m_itemList.size(); i++){
		if(strcmp(m_itemList[i].material, name.c_str())==0){
			cost = m_itemList[i].cost;
			break;
		}
	}
	heroPackage.finance += cost*0.3;
	delFromPackage(m_wdj, name.c_str()); //添加进包裹
	m_doExchanging = false;
	if(m_wdj==0){ //为了更新列表信息
		onRight();
		onLeft();
	}else{
		onLeft();
		onRight();
	}
	delete m_label_money;
	stringstream total;
	total << "$";
	total << heroPackage.finance;
	m_label_money = new GUI::GUILabel("font\\simkai.ttf", 16, total.str(),
		255, 255, 128, SHOP_STARTX+45, SHOP_STARTY+340);
}
