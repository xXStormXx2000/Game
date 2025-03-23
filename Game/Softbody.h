#pragma once
#include "System.h"
#include "DynamicArray.h"
#include "unordered_map"
#include "spring.h"
#include "SDL.h"
#include "Debug.h"
#include "SDL_image.h"
#include "somData.h"

class Softbody : public System {
public:
	void onCollision(const CollisionEvent&);
	void update(Entity);
};

