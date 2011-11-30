#include "../include/game.h"

HeroSet heroSet; //人物集合
Package heroPackage; //人物的包裹
bool gamePause;
bool gameOver;
int gameChapter;


/*******************全局的辅助函数************************/
string newLine(string str, int n){
	int index = 0, k = 0;
	while(index<str.size()){
		if(str.c_str()[index]>=0){//非汉字
			index++;
			k++; //一个字符
		}
		else{ //汉字
			index+=2;
			k+=2;
		}
		if(k>2*n){//需要换行
			str = str.insert(index, "\n");
			index++;
			k = 0;
		}
	}
	return str;
}

void addToPackage(int witch, string material){
	vector<Mate>* point;
	switch(witch){
		case 0:
			point = &heroPackage.weapon;
			break;
		case 1:
			point = &heroPackage.drug;
			break;
		case 2:
			point = &heroPackage.jewelry;
			break;
		case 3:
			point = &heroPackage.story;
			break;
	}
	int i;
	for(i=0; i<(*point).size(); i++){
		if(strcmp((*point)[i].material, material.c_str())==0){
			(*point)[i].num++; //增加数量	
			break;
		}
	}
	if(i>=(*point).size()){//包裹中还没有改物品,新增
		Mate t;
		strcpy(t.material, material.c_str());
		t.num = 1;
		(*point).push_back(t);
	}
}

void delFromPackage(int witch, string material){
	vector<Mate>* point;
	switch(witch){
		case 0:
			point = &heroPackage.weapon;
			break;
		case 1:
			point = &heroPackage.drug;
			break;
		case 2:
			point = &heroPackage.jewelry;
			break;
		case 3:
			point = &heroPackage.story;
			break;
	}
	int i;
	vector<Mate>::iterator iter = (*point).begin();
	while( iter!=(*point).end() ){
		if(strcmp((*iter).material, material.c_str())==0){
			(*iter).num--; //减少数量
			if((*iter).num==0)iter = (*point).erase(iter);
			break;
		}else iter++;
	}	
}

void addToHero(int nameid, string material){
	vector<Mate>& point = heroSet.items[nameid];
	Mate t;
	strcpy(t.material, material.c_str());
	t.num = 1;
	point.push_back(t);
}

void delFromHero(int nameid, string material){
	vector<Mate>& point = heroSet.items[nameid];
	vector<Mate>::iterator iter = point.begin();
	while( iter!=point.end() ){
		if(strcmp((*iter).material, material.c_str())==0){
			(*iter).num--; //减少数量
			if((*iter).num==0)iter = point.erase(iter);
			break;
		}else iter++;
	}	
}

bool hasGood(string material){
	for(int i=0; i<heroPackage.weapon.size(); i++){
		if(!strcmp(heroPackage.weapon[i].material, material.c_str()))return true;
	}
	for(int i=0; i<heroPackage.jewelry.size(); i++){
		if(!strcmp(heroPackage.jewelry[i].material, material.c_str()))return true;
	}
	for(int i=0; i<heroPackage.drug.size(); i++){
		if(!strcmp(heroPackage.drug[i].material, material.c_str()))return true;
	}
	for(int i=0; i<heroPackage.story.size(); i++){
		if(!strcmp(heroPackage.story[i].material, material.c_str()))return true;
	}
	return false;
}