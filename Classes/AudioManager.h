#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER

#include "cocos2d.h"
#include "AudioEngine.h"

namespace Retry {

namespace {
struct AudioInfo {
	int id;
	float volume;
};
}
using namespace cocos2d;
using namespace experimental;

class AudioManager {
private:
	static AudioManager* instance;
	AudioManager() = default;
public:
	static AudioManager* getInstance();

	void setMasterVolume(float vol);
	void muteAll();
	void unmuteAll();

	void playMusic(std::string path, bool repeat = true, float vol = 1.0f);
	void stopMusic();
	void pauseMusic();
	void resumeMusic();
	void setMusicVolume(float vol);

	unsigned int playEffect(std::string path, bool repeat = false, float vol = 1.0f);

	void stopEffect(std::string path, bool stopAll = false);
	void stopEffect(std::string path, int index);

	void pauseEffect(std::string path, bool pauseAll = false);
	void pauseEffect(std::string path, int index);

	void resumeEffect(std::string path, bool resumeAll = false);
	void resumeEffect(std::string path, int index);

	void setEffectVolume(std::string path, float vol, bool affectAll = false);
	void setEffectVolume(std::string path, float vol, int index);

	void stopAllEffects();
	void pauseAllEffects();
	void resumeAllEffects();

private:
	static AudioInfo music;
	static std::unordered_map<std::string, std::vector<AudioInfo>> effects;

	static float masterVolume;

	static bool mute;
};
}

#endif
