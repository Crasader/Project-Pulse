#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER

#include "cocos2d.h"

class AudioManager {
private:
	static AudioManager* instance;
	AudioManager() = default;
public:
	static AudioManager* getInstance();


private:


};

#endif
