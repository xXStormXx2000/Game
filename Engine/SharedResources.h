#pragma once
#include "DynamicArray.h"
#include "CollisionEvent.h"
#include <unordered_set>
#include <unordered_map>
#include "SDL_mouse.h"
#include <filesystem>


class SharedResources {
	CollisionEventMap collisionEvents;
	float deltaTime; //s

	std::filesystem::path sceneFilePath;
	bool sceneChange = false;

	char keysPressed[32];
	char keysDown[32];
	char keysReleased[32];

	int mouseX, mouseY;
	uint32_t mouseButtonPressed;
	uint32_t mouseButtonDown;
	uint32_t mouseButtonReleased;


	std::unordered_map<std::string, std::unordered_set<Entity>> tagEntityLookup;

	int windowWidth;
	int windowHeight;

	std::string textInput;
	bool textInputSate = false;

public:

	void updateMouseState();

	Vector3D getMousePos();

	bool leftMouseButtonPressed();
	bool middleMouseButtonPressed();
	bool rightMouseButtonPressed();

	bool leftMouseButtonDown();
	bool middleMouseButtonDown();
	bool rightMouseButtonDown();

	bool leftMouseButtonReleased();
	bool middleMouseButtonReleased();
	bool rightMouseButtonReleased();


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

	const std::filesystem::path& getSceneFilePath();
	void setSceneFilePath(const std::filesystem::path&);

	bool getSceneChange();
	void setSceneChange(bool);

	void addEntityTag(const std::string&, Entity);
	void removeEntityTag(const std::string&, Entity);
	const std::unordered_set<Entity>& getTagEntitys(const std::string&);

	void setWindowSize(int, int);

	int getWindowWidth();
	int getWindowHeight();

	std::string getTextInput();
	void concatenateTextInput(const std::string&);
	void setTextInput(const std::string&);
	void popBackTextInput();

	void toggleTextInputState();
	bool getTextInputState();
};

