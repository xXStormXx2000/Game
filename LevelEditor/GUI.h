#pragma once
#include "System.h"
#include <filesystem>
class GUI : public System {
	std::filesystem::path gamePath;
public:
	void setGamePath(const std::filesystem::path&);
};

