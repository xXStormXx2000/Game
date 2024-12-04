#pragma once
#include "System.h"
#include "DynamicArray.h"
#include "spring.h"
#include "SDL.h"
#include "Debug.h"
#include "SDL_image.h"
class Softbody : public System {
	DynamicArray<Entity> softbodys;
public:
	void onCollision(const CollisionEvent&);
	void start(Entity);
	void preUpdate(Entity);
	void update(Entity);
};

