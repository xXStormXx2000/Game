#pragma once
#include "SDL.h"
#include "SDL_mixer.h"	
#include "Debug.h"
#include <string>
#include <unordered_map>


class AudioManager {
	SDL_AudioDeviceID audioDevice;
	std::unordered_map<std::string, Mix_Chunk*> audioMap;
	std::unordered_map<std::string, Mix_Music*> musicMap;
	bool initFeil = false;

public:
	AudioManager();

	bool failure();

	void addSound(std::string, std::string);

	void playSound(std::string);

	~AudioManager();
};

