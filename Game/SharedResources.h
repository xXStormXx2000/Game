#pragma once
#include "DynamicArray.h"
#include "CollisionEvent.h"


class SharedResources {
	CollisionEventMap collisionEvents;
	double deltaTime;// s
public:
	DynamicArray<CollisionEvent> getCollisionEvents(int);
	void setCollisionEvents(CollisionEventMap);
	
	void setDeltaTime(double);
	double getDeltaTime();
};

