// RPG_Sound( 2009 )
//
//游戏音频播放类
//


#ifndef MIX_SOUND_CLASS_H
#define MIX_SOUND_CLASS_H

#include <iostream>
#include <string>
#include "SDL\SDL.h"
#include "SDL\SDL_mixer.h"

namespace RPG_NS_MixSound{
	class BaseMixSound//音频播放基类
	{
	private:
		static int MixNUM;
	protected:
		BaseMixSound();
	public:
		virtual ~BaseMixSound();
	};

	class EffectSound: public BaseMixSound	//音效类
	{
	private:
		Mix_Chunk* sound;
	public:
		EffectSound(const std::string& sound_fileName);
		~EffectSound();
		void play() const;
	};

	class MusicSound: public BaseMixSound	//背景音乐类
	{
	private:
		Mix_Music* music;
	public:
		MusicSound(const std::string& music_fileName);
		~MusicSound();
		void play() const;
		void stop() const;
	};
}
#endif