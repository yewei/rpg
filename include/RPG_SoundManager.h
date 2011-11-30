#ifndef HEADER_SOUNDMANAGER_H
#define HEADER_SOUNDMANAGER_H
#include <map>
#include <string>
#include "SDL\SDL.h"
#include "SDL\SDL_mixer.h"
#include "RPG_MixSound.h"
using RPG_NS_MixSound::EffectSound;
using RPG_NS_MixSound::MusicSound;

namespace RPG_NS_SoundManager{
	class RPG_SoundManager
	{
	private:
		static EffectSound* effectSnd;//音效播放器
		static MusicSound* musicSnd;//背景音乐播放器
		static bool bMusicON;	//音乐播放是否进行中
		static bool bDisableMusic; //是否禁止播放音乐
		static int currentID;//当前音乐播放ID号
		//static bool isFirst;
	public:
		static bool isMusicON() {return bMusicON;}
		static void setMusicON(bool on) {bMusicON = on;}
		static bool isDisableMusic() {return bDisableMusic;}
		static void disableMusic(bool disable) {bDisableMusic = disable;}

		static std::map<int, std::string> musicList;	//地图对应的音乐文件列表
	//
		static void AudioManager_Init();	//音频管理器初始化	
		static void playSnd(const int musicID);//播放音乐
		static void playSnd(const std::string &musicFileName);
		static void stopMusic();//停止播放音乐
		static void clearEft();//清楚音效播放残留
	public:
		/*
		==============音乐ID定义
		*/
		//天气类型
		static const int CLIMATE_BASE = 0x100000;
		static const int SUN = CLIMATE_BASE+0x0000;//晴
		static const int RAIN = CLIMATE_BASE+0x0001;//雨天
		static const int THUNDER = CLIMATE_BASE+0x0002;//雷天
		static const int DUSK = CLIMATE_BASE+0x0004;//黄昏
		static const int DARK = CLIMATE_BASE+0x0008;//晚上
		static const int WINDY = CLIMATE_BASE+0x00016;//挂风
		//音效ID
		static const int EFFECT_BASE = 0x1000000;
		static const int WORDSND = EFFECT_BASE + 0x0000;//对话系统说话音效
		static const int BTN1 = EFFECT_BASE + 0x0001;//按钮1
		static const int BTN2 = EFFECT_BASE + 0x0002;//

	public:
		RPG_SoundManager(void);
	public:
		~RPG_SoundManager(void);
	};
}
#endif