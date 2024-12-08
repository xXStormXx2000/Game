#pragma once

#include "SDL.h"
#include "Renderer.h"
#include "Debug.h"
#include "Scene.h"
#include "Timer.h"
#include "FileManager.h"
#include "PhysicsEngine.h"
#include "SharedResources.h"
#include "DynamicArray.h"
#include "AudioManager.h"

#include "System.h"

#include <thread>


class Application {
	SDL_Window* window;
	Renderer renderer;
	AudioManager audioManager;
	bool appRunning = false;

	Scene scene;
	bool sceneChange = false;

	DynamicArray<System*> systems = {};

	Timer timer;

	int fps;
	float fpsLow = 100000000;

	FileManager fileManager;

	PhysicsEngine physicsEngine;

	SharedResources sharedResources;


public:

	Application(const char*, int, int, int);
	bool getRunning();

	void run();

	void addSystem(System*);

	void start();

	void handleEvents();

	void collisionHandling();

	void update();

	void render();

	void end();

	bool getSceneChange();
	
	~Application();
};

