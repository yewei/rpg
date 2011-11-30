#define IN_NPC_DEBUG

#include "../include/RPG_NPC.h"

RPG_NS_NPC::RPG_NPC::RPG_NPC(std::string imageFileName, RPG_NS_Map::RPG_Map *pMap){
	m_image = new RPG_NS_Image::RPG_Image(imageFileName);
	m_map = pMap;
	m_belongsToMap = pMap->getMapID();
	m_bBusy = false;
	m_bHide = false;
	m_bDead = false;
	m_currentFrame = 0;
	m_moveSpeed = NPC_UPDATE_DELAY;
	m_image->setColorKey(255, 0, 255);
	m_tileW = pMap->getTiledSize().width;
	m_tileH = pMap->getTiledSize().height;
	tc = upFre = 0;
}

RPG_NS_NPC::RPG_NPC::RPG_NPC(string imageFileName, int mapID){
	m_image = new RPG_NS_Image::RPG_Image(imageFileName);
	m_map = NULL;
	m_belongsToMap = mapID;
	m_bBusy = false;
	m_bHide = false;
	m_bDead = false;
	m_currentFrame = 0;
	m_moveSpeed = NPC_UPDATE_DELAY;
	m_image->setColorKey(255, 0, 255);
	//m_tileW = pMap->getTiledSize().width;
	//m_tileH = pMap->getTiledSize().height;
	m_tileW = 0;
	m_tileH = 0;
	tc = upFre = 0;
}

#ifdef IN_NPC_DEBUG
RPG_NS_NPC::RPG_NPC::RPG_NPC(std::string imageFileName){ //for debug
	m_image = new RPG_NS_Image::RPG_Image(imageFileName);
	m_bBusy = false;
	m_bHide = false;
	m_bDead = false;
	m_currentFrame = 0;
	m_moveSpeed = NPC_UPDATE_DELAY;
	m_image->setColorKey(255, 0, 255);
	tc = upFre = 0;
}
#endif

RPG_NS_NPC::RPG_NPC::~RPG_NPC(){
	delete m_image;
}

void RPG_NS_NPC::RPG_NPC::splitImage(int cX, int cY){
	m_image->splitImage(cX, cY);
}

void RPG_NS_NPC::RPG_NPC::addDirMotion(int dir, int index){
	switch(dir){
		case NPC_UP:
			m_upMotion.push_back(index);
			break;
		case NPC_DOWN:
			m_downMotion.push_back(index);
			break;
		case NPC_LEFT:
			m_leftMotion.push_back(index);
			break;
		case NPC_RIGHT:
			m_rightMotion.push_back(index);
	}
}

void RPG_NS_NPC::RPG_NPC::setPos(int x, int y){
	m_posX = x;
	m_posY = y;
	m_oldX = x;
	m_oldY = y;
}

void RPG_NS_NPC::RPG_NPC::setPosE(int x, int y){
	m_posXE = x;
	m_posYE = y;
}

void RPG_NS_NPC::RPG_NPC::setTileSize(int w, int h){
	m_tileW = w;
	m_tileH = h;
}

void RPG_NS_NPC::RPG_NPC::setMoveSpeed(int s){
	m_moveSpeed = s;
}

void RPG_NS_NPC::RPG_NPC::setHide(bool isHide){
	m_bHide = isHide;
}

void RPG_NS_NPC::RPG_NPC::setDead(bool isDead){
	m_bDead = isDead;
}

void RPG_NS_NPC::RPG_NPC::setMoveStatus(int status){
	if(m_upMotion.size()!=8 || m_downMotion.size()!=8
		|| m_leftMotion.size()!=8 || m_rightMotion.size()!=8)return;
	m_moveStatus = status;
	switch(status){
		case NPC_UP:
			m_currentFrame = m_upMotion[3];
			break;
		case NPC_DOWN:
			m_currentFrame = m_downMotion[3];
			break;
		case NPC_LEFT:
			m_currentFrame = m_leftMotion[3];
			break;
		case NPC_RIGHT:
			m_currentFrame = m_rightMotion[3];
			break;
	}
}

void RPG_NS_NPC::RPG_NPC::setNPCID(int id){
	m_NPCID = id;
}

void RPG_NS_NPC::RPG_NPC::setBelongsToMap(int map){
	m_belongsToMap = map;
}

void RPG_NS_NPC::RPG_NPC::setNPCUnicode(int uni){
	m_NPCUnicode = uni;
}

void RPG_NS_NPC::RPG_NPC::setMap(RPG_NS_Map::RPG_Map* map){
	//将地图相关的全部信息更新
	m_map = map;
	m_belongsToMap = map->getMapID();
	m_tileW = map->getTiledSize().width;
	m_tileH = map->getTiledSize().height;
}

void RPG_NS_NPC::RPG_NPC::moveUp(){
#ifndef IN_NPC_DEBUG
	if(m_bBusy || m_map->canMov(m_posX, m_posY-1))return;
#else
	if(m_bBusy)return; //for debug
#endif
	setMoveStatus(NPC_UP); //更改朝向
	if(!m_map->canMov(m_posX, m_posY-1))return;
	//setPos(m_posX, m_posY-1);
	m_posY -= 1;
	m_moveStatus = NPC_UP;
	m_bBusy = true;
}

void RPG_NS_NPC::RPG_NPC::moveDown(){
#ifndef IN_NPC_DEBUG
	if(m_bBusy || m_map->canMov(m_posX, m_posY+1))return;
#else
	if(m_bBusy)return;
#endif
	setMoveStatus(NPC_DOWN);
	if(!m_map->canMov(m_posX, m_posY+1))return;
	//setPos(m_posX, m_posY+1);
	m_posY += 1;
	m_moveStatus = NPC_DOWN;
	m_bBusy = true;
}

void RPG_NS_NPC::RPG_NPC::moveLeft(){
#ifndef IN_NPC_DEBUG
	if(m_bBusy || m_map->canMov(m_posX-1, m_posY))return;
#else
	if(m_bBusy)return;
#endif
	setMoveStatus(NPC_LEFT);
	if(!m_map->canMov(m_posX-1, m_posY))return;
	//setPos(m_posX-1, m_posY);
	m_posX -= 1;
	m_moveStatus = NPC_LEFT;
	m_bBusy = true;
}

void RPG_NS_NPC::RPG_NPC::moveRight(){
#ifndef IN_NPC_DEBUG
	if(m_bBusy || m_map->canMov(m_posX+1, m_posY))return;
#else
	if(m_bBusy)return;
#endif
	setMoveStatus(NPC_RIGHT);
	if(!m_map->canMov(m_posX+1, m_posY))return;
	//setPos(m_posX+1, m_posY);
	m_posX += 1;
	m_moveStatus = NPC_RIGHT;
	m_bBusy = true;
}

void RPG_NS_NPC::RPG_NPC::move(int dir){
	switch(dir){
		case NPC_UP:
			moveUp();
			break;
		case NPC_DOWN:
			moveDown();
			break;
		case NPC_LEFT:
			moveLeft();
			break;
		case NPC_RIGHT:
			moveRight();
			break;
	}
}

void RPG_NS_NPC::RPG_NPC::update(){ //需要更新16次才能完成一个命令,每两次更新脚步
	if(!m_bBusy)return;
	if(++upFre < m_moveSpeed)return;
	upFre = 0;
	if(m_posXE < m_posX*m_tileW){
		//当前位置偏左
		if(tc==15){
			m_posXE += m_tileW/16 + m_tileW%16;//最后一步要保证走到终点
		}
		else{
			m_posXE += m_tileW/16;
		}
		if(tc%2)m_currentFrame = m_rightMotion[tc/2];
	}
	else if(m_posXE > m_posX*m_tileW){
		//当前位置偏右
		if(tc==15){
			m_posXE -= m_tileW/16 + m_tileW%16;
		}
		else{
			m_posXE -= m_tileW/16;
		}
		if(tc%2)m_currentFrame = m_leftMotion[tc/2];
	}
	else if(m_posYE < m_posY*m_tileH){
		//当前位置偏上
		if(tc==15){
			m_posYE += m_tileH/16 + m_tileH%16;
		}
		else{
			m_posYE += m_tileH/16;
		}
		if(tc%2)m_currentFrame = m_downMotion[tc/2];
	}else if(m_posYE > m_posY*m_tileH){
		//当前位置偏低
		if(tc==15){
			m_posYE -= m_tileH/16 + m_tileH%16;
		}
		else{
			m_posYE -= m_tileH/16;
		}
		if(tc%2)m_currentFrame = m_upMotion[tc/2];
	}
	if(++tc==16){ //一个命令执行完毕
		m_oldX = m_posX;
		m_oldY = m_posY;
		tc = 0;
		m_bBusy = false;
	}

}

void RPG_NS_NPC::RPG_NPC::undoCommand(){
	m_posX = m_oldX;
	m_posY = m_oldY;
	m_bBusy = false;
	tc = 0;
	upFre = 0;
}

bool RPG_NS_NPC::RPG_NPC::renderNPC(SDL_Surface* surfDest){
	return m_image->drawSpliter(surfDest, m_posXE, m_posYE, m_currentFrame);
}
