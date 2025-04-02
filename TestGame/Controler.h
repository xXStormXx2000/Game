#pragma once
#include <System.h>
class Controler :  public System {
	DynamicArray<float> fpsArray;
	float fps = 0;
	int fpsIndex = 0;
public:
	Controler();	
	void onCollision(const CollisionEvent&);
	void start(Entity);
	void update(Entity);
};

