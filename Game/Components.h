#pragma once
#include "Vector3D.h"
#include "SDL.h"

struct Transform {
	Vector3D position = { 0, 0, 0 }; // pixels
	Vector3D velocity = { 0, 0, 0 }; // pixels/s
	Vector3D rotation = { 0, 0, 0 }; // rad
	Vector3D scale = { 1, 1, 1 };
};

struct Collider {
	Vector3D Offset = { 0,0,0 }; // pixels
	double width = 0, height = 0; // pixels
};

struct Rigidbody {
	float mass = 1; // kg
	float density = 1; // kg/pixels^3
	float friction = 0;
	float restitution = 0;
	Vector3D centerOfMass = { 0, 0, 0 }; // pixels
	Vector3D acceleration = { 0, 0, 0 }; // pixels/s^2
	Vector3D angularVelocity = { 0, 0, 0 }; // rad/s
};

struct Sprite {
	Vector3D Offset = { 0,0,0 }; // pixels
	double width = 0, height = 0; // pixels
	SDL_Rect texturePortion = { 0, 0, 0, 0 }; // pixels
	SDL_Texture* sprite = NULL;
};

enum Flags {
	Active,
	Visible,
	Muted,
	Solid,
	Static,
	MovedX,
	MovedY
};
enum ComponentFlags {
	TransformFlag,
	ColliderFlag,
	RigidbodyFlag,
	SpriteFlag
};

struct EntityFlags {
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

	/*
	0b0000'0001 = Transform
	0b0000'0010 = Collider
	0b0000'0100 = Rigidbody
	0b0000'1000 = Sprite
	*/
	int components;

	bool getFlag(Flags);
	void setFlag(Flags, bool);
	bool checkFlags(int);

	bool haveComponent(ComponentFlags);
	bool checkComponents(int);
};