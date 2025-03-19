#pragma once
#include "DynamicArray.h"
#include "CollisionEvent.h"


class SharedResources {
	CollisionEventMap collisionEvents;
	float deltaTime; //s

	char keysPressed[32];
	char keysDown[32];
	char keysReleased[32];
public:
	DynamicArray<CollisionEvent> getCollisionEvents(int);
	void setCollisionEvents(CollisionEventMap);
	
	void resetKeysPressed();
	bool getKeyPressed(char);
	void setKeyPressed(char);

	bool getKeyDown(char);
	void setKeyDown(char, bool);

	void resetKeysReleased();
	bool getKeyReleased(char);
	void setKeyReleased(char);

	void setDeltaTime(float);
	float getDeltaTime();
};

