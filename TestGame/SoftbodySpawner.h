#pragma once
#include "System.h"
#include "DynamicArray.h"
#include "unordered_map"
#include "spring.h"
#include "Debug.h"
#include "somData.h"

class SoftbodySpawner : public System {
	DynamicArray<DynamicArray<Entity>> softbodys;
	std::unordered_map<int, std::pair<int, int>> posMap;
public:
	SoftbodySpawner();
	void start(Entity);
	void preUpdate(Entity);
};

