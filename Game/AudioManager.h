#pragma once
#include "SDL.h"
#include "Debug.h"

class AudioManager {
	bool initFeil = false;
public:
	AudioManager();
	bool failure();
	void playSound();
};

