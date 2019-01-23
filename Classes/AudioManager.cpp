#include "AudioManager.h"

using namespace Retry;

AudioManager* AudioManager::instance = 0;

AudioInfo AudioManager::music;
std::unordered_map<std::string, std::vector<AudioInfo>> AudioManager::effects;

float AudioManager::masterVolume = 1.0f;

bool AudioManager::mute = false;

AudioManager* AudioManager::getInstance() {
	return instance == 0 ? (instance = new AudioManager()) : instance;
}

void AudioManager::setMasterVolume(float vol) {
	vol = vol < 0 ? 0 : (vol > 1.0f ? 1.0f : vol);
	vol = abs(log10(1 - 0.9f * vol));
	if ((int) AudioEngine::getState(music.id) > 0)
		AudioEngine::setVolume(music.id, music.volume * vol * !mute);
		for (auto i : effects) {
			for (auto j : i.second) {
				AudioEngine::setVolume(j.id, j.volume * vol * !mute);
			}
		}
	masterVolume = vol;
}

void AudioManager::muteAll() {
	if (mute) return;
	mute = true;
	setMasterVolume(masterVolume);
}

void AudioManager::unmuteAll() {
	if (!mute) return;
	mute = false;
	setMasterVolume(masterVolume);
}

void AudioManager::playMusic(std::string path, bool repeat, float vol) {
	music.id = AudioEngine::play2d(path, repeat, vol  * !mute);
	music.volume = vol;
}

void AudioManager::stopMusic() {
	AudioEngine::stop(music.id);
}

void AudioManager::pauseMusic() {
	AudioEngine::pause(music.id);
}

void AudioManager::resumeMusic() {
	AudioEngine::resume(music.id);
}

void AudioManager::setMusicVolume(float vol) {
	vol = vol < 0 ? 0 : vol > 1.0f ? 1.0f : vol;
	music.volume = vol;
	AudioEngine::setVolume(music.id, masterVolume * music.volume * (int) !mute);
}

unsigned int AudioManager::playEffect(std::string path, bool repeat, float vol) {
	AudioInfo ai;
	ai.id = AudioEngine::play2d(path, repeat, vol * (int) !mute);
	ai.volume = vol;
	AudioEngine::setFinishCallback(ai.id, [path](int i, std::string s) {
		auto list = &effects[path];
		for (unsigned int c = 0; c < list->size(); c++) {
			if ((*list)[c].id == i) {
				list->erase(list->begin() + c);
			}
		}
	});
	if (effects.find(path) == effects.end()) {
		effects[path] = { ai };
	} else {
		effects[path].push_back(ai);
	}
	return ai.id;
}

void AudioManager::stopEffect(std::string path, bool stopAll) {
	if (effects.find(path) == effects.end()) return;

	auto list = &effects[path];

	for (unsigned int i = 0; i < list->size(); i++) {
		AudioEngine::pause((*list)[i].id);
		if (!stopAll) return;
	}
}

void AudioManager::stopEffect(std::string path, int index) {
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;

	AudioEngine::stop(effects[path][index].id);
}

void AudioManager::pauseEffect(std::string path, bool pauseAll) {
	if (effects.find(path) == effects.end()) return;

	auto list = &effects[path];

	for (unsigned int i = 0; i < list->size(); i++) {
		if (AudioEngine::getState((*list)[i].id) == AudioEngine::AudioState::PAUSED) continue;
		AudioEngine::pause((*list)[i].id);
		if (!pauseAll) return;
	}
}

void AudioManager::pauseEffect(std::string path, int index) {
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;

	AudioEngine::pause((effects[path])[index].id);
}

void AudioManager::resumeEffect(std::string path, bool resumeAll) {
	if (effects.find(path) == effects.end()) return;

	auto list = &effects[path];

	for (unsigned int i = 0; i < list->size(); i++) {
		if (AudioEngine::getState((*list)[i].id) != AudioEngine::AudioState::PAUSED) continue;
		AudioEngine::resume((*list)[i].id);
		if (!resumeAll) return;
	}
}

void AudioManager::resumeEffect(std::string path, int index) {
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;

	AudioEngine::resume(effects[path][index].id);
}

void AudioManager::setEffectVolume(std::string path, float vol, bool affectAll) {
	if (effects.find(path) == effects.end()) return;

	vol = vol < 0 ? 0 : vol > 1.0f ? 1.0f : vol;

	auto list = &effects[path];

	for (unsigned int i = 0; i < (affectAll ? list->size() : 1); i++) {
		AudioEngine::setVolume((*list)[i].id, masterVolume * vol * (int) !mute);
		(*list)[i].volume = vol;
	}
}

void AudioManager::setEffectVolume(std::string path, float vol, int index) {
	if (effects.find(path) == effects.end()) return;
	if (index < 0 || index >= (signed) effects[path].size()) return;
	vol = vol < 0 ? 0 : vol > 1.0f ? 1.0f : vol;

	AudioEngine::setVolume(effects[path][index].id, masterVolume * vol * (int) !mute);
	effects[path][index].volume = vol;
}

void AudioManager::stopAllEffects() {
	for (auto i : effects)
		for (auto j : i.second)
			AudioEngine::stop(j.id);
}

void AudioManager::pauseAllEffects() {
	for (auto i : effects)
		for (auto j : i.second)
			AudioEngine::pause(j.id);
}

void AudioManager::resumeAllEffects() {
	for (auto i : effects)
		for (auto j : i.second)
			AudioEngine::resume(j.id);
}