#pragma once
#include "System.h"
#include "DynamicArray.h"
#include "unordered_map"
#include "spring.h"
#include "SDL.h"
#include "Debug.h"
#include "SDL_image.h"

class Softbody : public System {
	DynamicArray<DynamicArray<Entity>> softbodys;
	std::unordered_map<int, std::pair<int, int>> posMap;
	int i = 0;
	int j = 0;
public:
	Softbody();
	void onCollision(const CollisionEvent&);
	void start(Entity);
	void preUpdate(Entity);
	void update(Entity);
};

