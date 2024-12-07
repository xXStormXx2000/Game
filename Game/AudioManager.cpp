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
