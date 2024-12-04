#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Renderer.h"
#include <string>
#include "DynamicArray.h"
#include "System.h"
#include <fstream>
#include "PhysicsEngine.h"

class FileManager {
	int entityCount = 0;

	Scene* scene = nullptr;

	Renderer* renderer = nullptr;

	PhysicsEngine* physicsEngine = nullptr;

	Entity createEntity();

public:
	FileManager(Scene* = nullptr, Renderer* = nullptr, PhysicsEngine* = nullptr);

	void loadScene(std::string, DynamicArray<System*>&);
};

