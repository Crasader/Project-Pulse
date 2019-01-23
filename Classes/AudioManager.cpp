#include "AudioManager.h"

#include "cocos2d.h"
#include "AudioEngine.h"

namespace Retry
{

AudioInfo Audio::music;
std::unordered_map<std::string, std::vector<AudioInfo>> Audio::effects;

float Audio::masterVolume = 1.0f;

bool Audio::mute = false;

void Audio::setMasterVolume(float vol)
{
	vol = vol < 0 ? 0 : (vol > 1.0f ? 1.0f : vol);
	vol = abs(log10(1 - 0.9f * vol));
	if ((int) cocos2d::experimental::AudioEngine::getState(music.id) > 0)
		cocos2d::experimental::AudioEngine::setVolume(music.id, music.volume * vol * !mute);
	for (auto i : effects)
	{
		for (auto j : i.second)
		{
			cocos2d::experimental::AudioEngine::setVolume(j.id, j.volume * vol * !mute);
		}
	}
	masterVolume = vol;
}

void Audio::muteAll()
{
	if (mute) return;
	mute = true;
	setMasterVolume(masterVolume);
}

void Audio::unmuteAll()
{
	if (!mute) return;
	mute = false;
	setMasterVolume(masterVolume);
}

void Audio::playMusic(std::string path, bool repeat, float vol)
{
	music.id = cocos2d::experimental::AudioEngine::play2d(path, repeat, vol * !mute);
	music.volume = vol;
}

void Audio::stopMusic()
{
	cocos2d::experimental::AudioEngine::stop(music.id);
}

void Audio::pauseMusic()
{
	cocos2d::experimental::AudioEngine::pause(music.id);
}

void Audio::resumeMusic()
{
	cocos2d::experimental::AudioEngine::resume(music.id);
}

void Audio::setMusicVolume(float vol)
{
	vol = vol < 0 ? 0 : vol > 1.0f ? 1.0f : vol;
	music.volume = vol;
	cocos2d::experimental::AudioEngine::setVolume(music.id, masterVolume * music.volume * (int) !mute);
}

unsigned int Audio::playEffect(std::string path, bool repeat, float vol)
{
	AudioInfo ai;
	ai.id = cocos2d::experimental::AudioEngine::play2d(path, repeat, vol * (int) !mute);
	ai.volume = vol;
	cocos2d::experimental::AudioEngine::setFinishCallback(ai.id, [path](int i, std::string s) {
		auto list = &effects[path];
		for (unsigned int c = 0; c < list->size(); c++)
		{
			if ((*list)[c].id == i)
			{
				list->erase(list->begin() + c);
			}
		}
	});
	if (effects.find(path) == effects.end())
	{
		effects[path] = { ai };
	} else
	{
		effects[path].push_back(ai);
	}
	return ai.id;
}

void Audio::stopEffect(std::string path, bool stopAll)
{
	if (effects.find(path) == effects.end()) return;

	auto list = &effects[path];

	for (unsigned int i = 0; i < list->size(); i++)
	{
		cocos2d::experimental::AudioEngine::pause((*list)[i].id);
		if (!stopAll) return;
	}
}

void Audio::stopEffect(std::string path, int index)
{
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;

	cocos2d::experimental::AudioEngine::stop(effects[path][index].id);
}

void Audio::pauseEffect(std::string path, bool pauseAll)
{
	if (effects.find(path) == effects.end()) return;

	auto list = &effects[path];

	for (unsigned int i = 0; i < list->size(); i++)
	{
		if (cocos2d::experimental::AudioEngine::getState((*list)[i].id) == cocos2d::experimental::AudioEngine::AudioState::PAUSED) continue;
		cocos2d::experimental::AudioEngine::pause((*list)[i].id);
		if (!pauseAll) return;
	}
}

void Audio::pauseEffect(std::string path, int index)
{
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;

	cocos2d::experimental::AudioEngine::pause((effects[path])[index].id);
}

void Audio::resumeEffect(std::string path, bool resumeAll)
{
	if (effects.find(path) == effects.end()) return;

	auto list = &effects[path];

	for (unsigned int i = 0; i < list->size(); i++)
	{
		if (cocos2d::experimental::AudioEngine::getState((*list)[i].id) != cocos2d::experimental::AudioEngine::AudioState::PAUSED) continue;
		cocos2d::experimental::AudioEngine::resume((*list)[i].id);
		if (!resumeAll) return;
	}
}

void Audio::resumeEffect(std::string path, int index)
{
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;

	cocos2d::experimental::AudioEngine::resume(effects[path][index].id);
}

void Audio::setEffectVolume(std::string path, float vol, bool affectAll)
{
	if (effects.find(path) == effects.end()) return;

	vol = vol < 0 ? 0 : vol > 1.0f ? 1.0f : vol;

	auto list = &effects[path];

	for (unsigned int i = 0; i < (affectAll ? list->size() : 1); i++)
	{
		cocos2d::experimental::AudioEngine::setVolume((*list)[i].id, masterVolume * vol * (int) !mute);
		(*list)[i].volume = vol;
	}
}

void Audio::setEffectVolume(std::string path, float vol, int index)
{
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;
	vol = vol < 0 ? 0 : vol > 1.0f ? 1.0f : vol;

	cocos2d::experimental::AudioEngine::setVolume(effects[path][index].id, masterVolume * vol * (int) !mute);
	effects[path][index].volume = vol;
}

void Audio::stopAllEffects()
{
	for (auto i : effects)
		for (auto j : i.second)
			cocos2d::experimental::AudioEngine::stop(j.id);
}

void Audio::pauseAllEffects()
{
	for (auto i : effects)
		for (auto j : i.second)
			cocos2d::experimental::AudioEngine::pause(j.id);
}

void Audio::resumeAllEffects()
{
	for (auto i : effects)
		for (auto j : i.second)
			cocos2d::experimental::AudioEngine::resume(j.id);
}
}