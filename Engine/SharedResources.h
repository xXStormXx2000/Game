#pragma once
#include "DynamicArray.h"
#include "CollisionEvent.h"
#include <unordered_set>
#include <unordered_map>


class SharedResources {
	CollisionEventMap collisionEvents;
	float deltaTime; //s

	std::string sceneFilePath;
	bool sceneChange = false;

	char keysPressed[32];
	char keysDown[32];
	char keysReleased[32];

	std::unordered_map<std::string, std::unordered_set<Entity>> tagEntityLookup;

	int windowWidth;
	int windowHeight;

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

	const std::string& getSceneFilePath();
	void setSceneFilePath(const std::string&);

	bool getSceneChange();
	void setSceneChange(bool);

	void addEntityTag(const std::string&, Entity);
	void removeEntityTag(const std::string&, Entity);
	const std::unordered_set<Entity>& getTagEntitys(const std::string&);

	void setWindowSize(int, int);

	int getWindowWidth();
	int getWindowHeight();
};

