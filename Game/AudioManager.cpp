#include "AudioManager.h"

AudioManager::AudioManager() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		debugMessage("SDL audio could not initialize! SDL_Error: " << SDL_GetError());
		this->initFeil = true;
	}
	if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID) !=
		(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID)) {
		debugMessage("SDL Mixer could not initialize! SDL_Error: " << Mix_GetError());
		this->initFeil = true;
		Mix_CloseAudio();
		Mix_Quit();
	}
	if (Mix_OpenAudio(48000, AUDIO_F32SYS, 1, 2048) == -1) {
		debugMessage("SDL Mixer could not initialize! SDL_Error: " << Mix_GetError());
		this->initFeil = true;
		Mix_CloseAudio();
		Mix_Quit();
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
	Mix_CloseAudio();
	Mix_Quit();
}
