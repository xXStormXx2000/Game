#pragma once
#include <fstream>
#include <string>
#include "DynamicArray.h"

#include "System.h"
#include "Entity.h"

#include "Scene.h"
#include "Renderer.h"
#include "PhysicsEngine.h"
#include "AudioManager.h"


class FileManager {

	Scene* scene = nullptr;

	Renderer* renderer = nullptr;

	PhysicsEngine* physicsEngine = nullptr;

	AudioManager* audioManager = nullptr;

	

public:
	void setScene(Scene*);
	void setRenderer(Renderer*);
	void setPhysicsEngine(PhysicsEngine*);
	void setAudioManager(AudioManager*);

	void loadScene(std::string, DynamicArray<System*>&, DynamicArray<Component*>&);
};

