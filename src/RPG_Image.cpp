#include <iostream>
#include "../include/RPG_Image.h"
using std::cout;
using std::endl;
RPG_NS_Image::RPG_Image::RPG_Image(string imageFileName){
    m_surfImage = NULL;
    SDL_Surface* surfTemp = NULL;

	if((surfTemp=SDL_LoadBMP(imageFileName.c_str()))==NULL){
		throw string("不能加载位图:")+imageFileName;
	}

	//m_surfImage = SDL_DisplayFormatAlpha(surfTemp);
	m_surfImage = SDL_DisplayFormat(surfTemp);
    SDL_FreeSurface(surfTemp);

	m_width = m_surfImage->w;
	m_height = m_surfImage->h;
	m_bpp = m_surfImage->format->BitsPerPixel;
	m_cX = m_cY = -1;
}

RPG_NS_Image::RPG_Image::~RPG_Image(){
    SDL_FreeSurface(m_surfImage);
}

bool RPG_NS_Image::RPG_Image::drawImage(SDL_Surface* surfDest, int destX, int destY){
    return drawImage(surfDest, destX, destY, 0, 0, m_width, m_height);
}

bool RPG_NS_Image::RPG_Image::drawImage(SDL_Surface* surfDest, int destX, int destY,
                            int srcX, int srcY, int srcWSize, int srcHSize){
	
	if(surfDest==NULL || m_surfImage==NULL)return false;
    SDL_Rect DestR, SrcR;
    DestR.x = destX;
    DestR.y = destY;
    DestR.w = srcWSize;
    DestR.h = srcHSize;
    SrcR.x = srcX;
    SrcR.y = srcY;
    SrcR.w = srcWSize;
    SrcR.h = srcHSize;
    SDL_BlitSurface(m_surfImage, &SrcR, surfDest, &DestR);
    return true;
}

void RPG_NS_Image::RPG_Image::splitImage(int cX, int cY){
	m_spliters.clear();
	m_cX = cX;
	m_cY = cY;
	SDL_Rect t;
	int lenX = m_width / cX; //每块宽度
	int lenY = m_height / cY; //每块长度
	t.w = lenX;
	t.h = lenY;
	for(int y=0; y<cY; y++)
		for(int x=0; x<cX; x++){
		t.x = x * lenX;
		t.y = y * lenY;
		m_spliters.push_back(t);
	}

}

bool RPG_NS_Image::RPG_Image::drawSpliter(SDL_Surface *surfDest, int destX, int destY, int index){
	if(index<0 || index > m_cX*m_cY-1)return false;
	return drawImage(surfDest, destX, destY, m_spliters[index].x,
						m_spliters[index].y, m_spliters[index].w, m_spliters[index].h);
	return true;
}

void RPG_NS_Image::RPG_Image::setColorKey(Uint8 r, Uint8 g, Uint8 b){
	m_colork = SDL_MapRGB(m_surfImage->format, r, g, b);
	if(SDL_SetColorKey(m_surfImage, SDL_SRCCOLORKEY, m_colork)<0)
		throw "不能设置透明色";
}

void RPG_NS_Image::RPG_Image::undoColorKey(){
	SDL_SetColorKey(m_surfImage, 0, m_colork);
}
