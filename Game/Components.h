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

struct EntityFlags {
	/*
	0b0000'0001 = Active
	0b0000'0010 = Visible
	0b0000'0100 = Muted
	0b0000'1000 = Solid
	0b0001'0000 = MovedX
	0b0010'0000 = MovedY
	*/
	int flags = 0b0001'1011;

	bool getActive();
	void setActive(bool);

	bool getVisible();
	void setVisible(bool);

	bool getMuted();
	void setMuted(bool);

	bool getSolid();
	void setSolid(bool);

	bool getMovedX();
	void setMovedX(bool);

	bool getMovedY();
	void setMovedY(bool);
};