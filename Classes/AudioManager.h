#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER

#include <string>
#include <unordered_map>

namespace Retry 
{

struct AudioInfo
{
	int id;
	float volume;
};

//using namespace cocos2d;
//using namespace experimental;

class Audio abstract
{
public:

	static void setMasterVolume(float vol);
	static void muteAll();
	static void unmuteAll();

	static void playMusic(std::string path, bool repeat = true, float vol = 1.0f);
	static void stopMusic();
	static void pauseMusic();
	static void resumeMusic();
	static void setMusicVolume(float vol);

	static unsigned int playEffect(std::string path, bool repeat = false, float vol = 1.0f);

	static void stopEffect(std::string path, bool stopAll = false);
	static void stopEffect(std::string path, int index);

	static void pauseEffect(std::string path, bool pauseAll = false);
	static void pauseEffect(std::string path, int index);

	static void resumeEffect(std::string path, bool resumeAll = false);
	static void resumeEffect(std::string path, int index);

	static void setEffectVolume(std::string path, float vol, bool affectAll = false);
	static void setEffectVolume(std::string path, float vol, int index);

	static void stopAllEffects();
	static void pauseAllEffects();
	static void resumeAllEffects();

private:
	static AudioInfo music;
	static std::unordered_map<std::string, std::vector<AudioInfo>> effects;

	static float masterVolume;

	static bool mute;
};
}

#endif
