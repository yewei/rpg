#ifndef HEADER_RPG_IMAGE_H
#define HEADER_RPG_IMAGE_H

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
using std::string;
using std::vector;

namespace RPG_NS_Image
{

	class RPG_Image
	{
		private:
			int m_width;
			int m_height;
			int m_bpp;
			SDL_Surface* m_surfImage;
			vector<SDL_Rect> m_spliters;
			int m_cX, m_cY;
			Uint32 m_colork;
	    
		public:
			RPG_Image(string imageFileName);
			~RPG_Image();
	        
		public:
			int getWidth() {return m_width;}
			int getHeight() {return m_height;}
			int getBpp() {return m_bpp;}
			SDL_Surface* surfImage() {return m_surfImage;}
			bool drawImage(SDL_Surface* surfDest, int destX, int destY); //整张图片blit到表面指定的位置 
			bool drawImage(SDL_Surface* surfDest, int destX, int destY,
							int srcX, int srcY, int srcWSize, int srcHSize); //部分图片blit到表面指定的位置
			void splitImage(int cX, int cY); //将Image切割成cY行cX列的许多小图象
			bool drawSpliter(SDL_Surface* surfDest, int destX, int destY, int index);
			void setColorKey(Uint8 r, Uint8 g, Uint8 b); //设置透明色
			void undoColorKey(); //取消透明色
	};      
}

#endif
