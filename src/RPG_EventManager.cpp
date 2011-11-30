#include "../include/RPG_EventManager.h"

//队列
vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_handleKeyState_list;

vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_onKeyDown_list;

vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_onLButtonDown_list;

vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_onRButtonDown_list;

vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_onLButtonUp_list;

vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_onRButtonUp_list;

vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_onMouseMove_list;

vector<RPG_NS_EventManager::RPG_EventHandleInterface*> 
	RPG_NS_EventManager::RPG_EventManager::m_onExit_list;

void RPG_NS_EventManager::RPG_EventManager::listen(){
	
	//handleKeyState
	Uint8* keystate = SDL_GetKeyState(NULL);
	for(int i=0; i<m_handleKeyState_list.size(); i++){
		m_handleKeyState_list[i]->handleKeyState(keystate);
	}

	SDL_Event ent;
	SDL_PollEvent(&ent);
	switch(ent.type){
		case SDL_QUIT: //关闭按钮
			for(int i=0; i<m_onExit_list.size(); i++){
				m_onExit_list[i]->onExit();
			}
			break;
		case SDL_KEYDOWN: //键盘事件
			for(int i=0; i<m_onKeyDown_list.size(); i++){
				m_onKeyDown_list[i]->onKeyDown(ent.key.keysym.sym);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(ent.button.button==SDL_BUTTON_LEFT){ //鼠标左键按下
				for(int i=0; i<m_onLButtonDown_list.size(); i++){
					m_onLButtonDown_list[i]->onLButtonDown(ent.button.x, ent.button.y);
				}
			}
			else if(ent.button.button==SDL_BUTTON_RIGHT){ //鼠标右键按下
				for(int i=0; i<m_onRButtonDown_list.size(); i++){
					m_onRButtonDown_list[i]->onRButtonDown(ent.button.x, ent.button.y);
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(ent.button.button==SDL_BUTTON_LEFT){ //鼠标左键放开
				for(int i=0; i<m_onLButtonUp_list.size(); i++){
					m_onLButtonUp_list[i]->onLButtonUp(ent.button.x, ent.button.y);
				}
			}
			else if(ent.button.button==SDL_BUTTON_RIGHT){ //鼠标右键放开
				for(int i=0; i<m_onRButtonUp_list.size(); i++){
					m_onRButtonUp_list[i]->onRButtonUp(ent.button.x, ent.button.y);
				}
			}
			break;
		case SDL_MOUSEMOTION: //鼠标移动
			for(int i=0; i<m_onMouseMove_list.size(); i++){
				m_onMouseMove_list[i]->onMouseMove(ent.motion.x, ent.motion.y);
			}
			break;
		default:
			;
	}
}

vector<RPG_NS_EventManager::RPG_EventHandleInterface*>& 
	RPG_NS_EventManager::RPG_EventManager::getByIndex(int sel){
		switch(sel){
			case EVENTMANAGER_HANDLEKEYSTATE:
				return m_handleKeyState_list;
			case EVENTMANAGER_ONKEYDOWN:
				return m_onKeyDown_list;
			case EVENTMANAGER_ONLBUTTONDOWN:
				return m_onLButtonDown_list;
			case EVENTMANAGER_ONRBUTTONDOWN:
				return m_onRButtonDown_list;
			case EVENTMANAGER_ONLBUTTONUP:
				return m_onLButtonUp_list;
			case EVENTMANAGER_ONRBUTTONUP:
				return m_onRButtonUp_list;
			case EVENTMANAGER_ONMOUSEMOVE:
				return m_onMouseMove_list;
			case EVENTMANAGER_ONEXIT:
				return m_onExit_list;
			default:
				;
		}
}

void RPG_NS_EventManager::RPG_EventManager::addListen(RPG_NS_EventManager::RPG_EventHandleInterface* handler, int sel){
	getByIndex(sel).push_back(handler);
}

void RPG_NS_EventManager::RPG_EventManager::delListen(RPG_NS_EventManager::RPG_EventHandleInterface* handler, int sel){
	vector<RPG_EventHandleInterface*>& list = getByIndex(sel);
	vector<RPG_EventHandleInterface*>::iterator iter = list.begin();
	for(; iter!=list.end(); ){
		if((*iter)==handler){
			iter = list.erase(iter);
		}
		else{
			iter++;
		}
	}
}

void RPG_NS_EventManager::RPG_EventManager::clearListen(int sel){
	getByIndex(sel).clear();	
}
