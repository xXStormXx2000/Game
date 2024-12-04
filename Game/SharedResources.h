#pragma once
#include "DynamicArray.h"
#include "CollisionEvent.h"


class SharedResources {
	CollisionEventMap collisionEvents;
	double deltaTime;// s

	char keysPressed[32];
public:
	DynamicArray<CollisionEvent> getCollisionEvents(int);
	void setCollisionEvents(CollisionEventMap);
	
	void resetKeysPressed();
	bool getKeyPressed(char);
	void setKeyPressed(char, bool);

	void setDeltaTime(double);
	double getDeltaTime();
};

