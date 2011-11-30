#include "../include/RPG_SoundManager.h"
#include "../include/RPG_SceneManager.h"
/* 音频管理器初始化文件soundlist.sndl 格式约定
mapID(地图ID号) musicFileName(字符串音乐文件)
音效ID musicFileName(字符串音乐文件)
天气类型ID musicFileName(字符串天气音乐文件)
-1（结束） END
*/

using std::string;

bool RPG_NS_SoundManager::RPG_SoundManager::bMusicON = false;
bool RPG_NS_SoundManager::RPG_SoundManager::bDisableMusic = false;
std::map<int, std::string> RPG_NS_SoundManager::RPG_SoundManager::musicList;
RPG_NS_MixSound::EffectSound* RPG_NS_SoundManager::RPG_SoundManager::effectSnd = NULL;
RPG_NS_MixSound::MusicSound* RPG_NS_SoundManager::RPG_SoundManager::musicSnd = NULL;
int RPG_NS_SoundManager::RPG_SoundManager::currentID = -1;
//bool RPG_NS_SoundManager::RPG_SoundManager::isFirst = true;

RPG_NS_SoundManager::RPG_SoundManager::RPG_SoundManager(void){

}

RPG_NS_SoundManager::RPG_SoundManager::~RPG_SoundManager(void){

}

void RPG_NS_SoundManager::RPG_SoundManager::AudioManager_Init(){//音频管理器初始化

	//读取 soundlist.sndl 文件：地图(ID)对应的背景音乐
	FILE *sndFile;
	string sndFileName = "data\\sound\\soundlist.sndl";
	sndFile = fopen(sndFileName.c_str(), "r");
	if(sndFile == NULL){
		throw "找不到音频管理器初始化文件："+sndFileName;
		return;
	}
	string musicFileName;
	char str[100];
	int ID;
	while(true){
		
		fscanf(sndFile, "%d %s", &ID, str);
		if(ID == -1) break;
		musicFileName = str;
		musicList[ID] = musicFileName;
	}
	fclose(sndFile);
	currentID = RPG_NS_SceneManager::RPG_SceneManager::getCurrentMap();
	currentID = -1;
}

void RPG_NS_SoundManager::RPG_SoundManager::playSnd(const int musicID){
	//地图和天气ID时以背景音乐模式播放，否则以音效模式播放
	//if(bMusicON && musicID<CLIMATE_BASE)
	//	stopMusic();
	//
	std::map<int, std::string>::iterator i = musicList.find(musicID);
    if(i==musicList.end()){
        return;
    }

	if(musicID >= CLIMATE_BASE){//音效
		if(effectSnd != NULL){
			delete effectSnd;
			effectSnd = NULL;
		}
		effectSnd = new EffectSound(musicList[musicID]);
		effectSnd->play();
	}
	else if(!bDisableMusic){	//背景音乐
		if(currentID!=-1 && !musicList[currentID].compare(musicList[musicID]))
			return;
		stopMusic();
		musicSnd = new MusicSound(musicList[musicID]);
		musicSnd->play();
		currentID = musicID;
		bMusicON = true;
	}

}

void RPG_NS_SoundManager::RPG_SoundManager::playSnd(const std::string &musicFileName){
	//通过音乐文件名播放背景音乐
	if(!bDisableMusic){
		stopMusic();
		musicSnd = new MusicSound(musicFileName);
		musicSnd->play();
		currentID = -1;
		bMusicON = true;
	}

}

void RPG_NS_SoundManager::RPG_SoundManager::stopMusic(){
	if(bMusicON){
		bMusicON = false;
		musicSnd->stop();
		delete musicSnd;
	}

}

void RPG_NS_SoundManager::RPG_SoundManager::clearEft(){
	if(effectSnd != NULL){
		delete effectSnd;
		effectSnd = NULL;
	}
}