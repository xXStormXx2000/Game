#pragma once
#include "System.h"
#include "DynamicArray.h"
#include <filesystem>

class Interface : public System {
	std::filesystem::path gamePath;
	std::filesystem::path sceneFile;
	DynamicArray<std::filesystem::path> findFiles(const std::filesystem::path&);

	DynamicArray<std::filesystem::path> artFiles;
	DynamicArray<std::filesystem::path> audioFiles;
	DynamicArray<std::filesystem::path> musicFiles;

	int select = -1;
	int blinkTime = 0;

	bool isNum(const std::string&);


	int sceneSizeX;
	int sceneSizeY;

	float cameraAspectRation;


public:
	void setGamePath(const std::filesystem::path&);
	void setSceneFile(const std::filesystem::path&);
	void start(Entity);
	void update(Entity);
};

