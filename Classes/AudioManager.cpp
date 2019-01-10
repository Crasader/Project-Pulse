#include "AudioManager.h"

AudioManager* AudioManager::instance = 0;

AudioManager* AudioManager::getInstance() {
	return instance == 0 ? (instance = new AudioManager()) : instance;
}