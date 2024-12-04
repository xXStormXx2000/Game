#pragma once
#include "Entity.h"
#include "Vector3D.h"
#include "unordered_map"
#include "DynamicArray.h"

struct CollisionEvent {
	Entity entity = { -1 };
	Entity other = { -1 };
	Vector3D collisionDirection = { 0, 0, 0 };
	double time = 1.01;
};

using CollisionEventMap = std::unordered_map<int, DynamicArray<CollisionEvent>>;