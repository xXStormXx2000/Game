#pragma once
#include "SDL.h"
#include "Debug.h"
#include <string>
#include <unordered_map>

struct Wave {
	SDL_AudioSpec audioSpec;
	Uint8* start;
	Uint32 length;
};

class AudioManager {
	SDL_AudioDeviceID audioDevice;
	std::unordered_map<std::string, Wave> audioMap;
	bool initFeil = false;

public:
	AudioManager();

	bool failure();

	void addSound(std::string, std::string);

	void playSound(std::string);

	~AudioManager();
};

