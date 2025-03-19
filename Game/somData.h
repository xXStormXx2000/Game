#pragma once
#include "Components.h"

class somData : public Component {
public:
	float data;
	Component* read_file(std::ifstream&, std::string&);
	void write_file(std::ofstream&);
};

