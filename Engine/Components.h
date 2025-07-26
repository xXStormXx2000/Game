#pragma once
#include "Vector3D.h"
#include "DynamicArray.h"
#include "SDL.h"
#include <fstream>
#include <typeinfo>
#include <typeindex>

class Component {
public:
	virtual Component* readFile(std::ifstream&, std::string&) = 0;
	virtual void writeFile(std::ofstream&) = 0;
	virtual ~Component() = default;
};


struct Transform : public Component {
	Vector3D position = { 0, 0, 0 }; // pixels
	Vector3D velocity = { 0, 0, 0 }; // pixels/s
	Vector3D scale = { 1, 1, 1 };
	Component* readFile(std::ifstream&, std::string&);
	void writeFile(std::ofstream&);
};

struct Collider : public Component {
	Vector3D offset = { 0,0,0 }; // pixels
	float width = 0, height = 0; // pixels
	Component* readFile(std::ifstream&, std::string&);
	void writeFile(std::ofstream&);
};

struct Sprite : public Component {
	Vector3D offset = { 0,0,0 }; // pixels
	float width = 0, height = 0; // pixels
	SDL_Rect texturePortion = { 0, 0, 0, 0 }; // pixels
	int spriteIndex = -1;
	Component* readFile(std::ifstream&, std::string&);
	void writeFile(std::ofstream&);
};

enum Flags {
	Active,
	Visible,
	Muted,
	Solid,
	Dynamic,
	MovedX,
	MovedY
};

class EntityFlags : public Component {
public:
	/*
	0b0000'0001 = Active
	0b0000'0010 = Visible
	0b0000'0100 = Muted
	0b0000'1000 = Solid
	0b0001'0000 = Static
	0b0010'0000 = MovedX
	0b0100'0000 = MovedY
	*/
	int flags;

	bool getFlag(int) const;
	void setFlag(int, bool);
	bool checkFlags(int) const;

	Component* readFile(std::ifstream&, std::string&);
	void writeFile(std::ofstream&);
};