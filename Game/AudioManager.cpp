#include "AudioManager.h"

AudioManager::AudioManager() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		debugMessage("SDL audio could not initialize! SDL_Error: " << SDL_GetError());
		this->initFeil = true;
	}
}

bool AudioManager::failure() {
	return this->initFeil;
}

void AudioManager::addSound(std::string name, std::string path){
	this->audioMap[name];
}

void AudioManager::playSound(std::string name) {
}

AudioManager::~AudioManager() {
	SDL_CloseAudioDevice(this->audioDevice);
}
