#pragma once
#include "System.h"
#include "DynamicArray.h"
#include "unordered_map"
#include "spring.h"
#include "Debug.h"
#include "somData.h"

class Softbody : public System {
	DynamicArray<DynamicArray<Entity>> softbodys;
	std::unordered_map<int, std::pair<int, int>> posMap;
public:
	Softbody();
	void start(Entity);
	void preUpdate(Entity);
	void update(Entity);
};

