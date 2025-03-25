#pragma once
#include "Components.h"

class somData : public Component {
public:
	float data;
	Component* readFile(std::ifstream&, std::string&);
	void writeFile(std::ofstream&);
};

