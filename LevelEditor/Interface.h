#pragma once
#include "System.h"
#include "DynamicArray.h"
#include <filesystem>
class Interface : public System {
	std::filesystem::path gamePath;
	DynamicArray<std::filesystem::path> findFiles(const std::filesystem::path&);

	DynamicArray<std::filesystem::path> artFiles;
	DynamicArray<std::filesystem::path> audioFiles;
	DynamicArray<std::filesystem::path> musicFiles;
public:
	void setGamePath(const std::filesystem::path&);
	void start(Entity);
};

