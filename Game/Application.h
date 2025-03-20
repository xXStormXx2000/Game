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



class Application {
	SDL_Window* window;
	Renderer renderer;
	AudioManager audioManager;
	bool appRunning = false;

	Scene scene;
	bool sceneChange = false;

	DynamicArray<System*> systems = {};
	DynamicArray<Component*> componentsTypes = {};

	Timer timer;

	int fps;

	FileManager fileManager;

	PhysicsEngine physicsEngine;

	SharedResources sharedResources;


public:

	Application(const char*, const char*, int, int, int);
	bool getRunning();

	void run();

	template<class T>
	void addSystem() {
		System* sys = new T;
		sys->setSystems(&this->systems);
		sys->setRenderer(&this->renderer);
		this->systems.pushBack(sys);
	}

	template<class T>
	void addComponent() {
		this->componentsTypes.pushBack(dynamic_cast<Component*>(new T));
	}

	void start();

	void handleEvents();

	void collisionHandling();

	void update();

	void render();

	void end();

	bool getSceneChange();
	
	~Application();
};

