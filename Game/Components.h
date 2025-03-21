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
	//Vector3D rotation = { 0, 0, 0 }; // rad
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

struct Rigidbody : public Component {
	float mass = 1; // kg
	float density = 1; // kg/pixels^3
	float friction = 0;
	float restitution = 0;
	Vector3D centerOfMass = { 0, 0, 0 }; // pixels
	Vector3D acceleration = { 0, 0, 0 }; // pixels/s^2
	//Vector3D angularVelocity = { 0, 0, 0 }; // rad/s
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

struct TileSet : public Component {
	Vector3D offset = { 0,0,0 }; // pixels
	float tileWidth = 0, tileHeight = 0; // pixels
	float textureTileWidth = 0, textureTileHeight = 0; // pixels
	int spriteIndex = -1;
	DynamicArray<Vector3D> tiles;
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