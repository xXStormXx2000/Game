#pragma once
#include <System.h>
class Controler :  public System {
public:
	void onCollision(const CollisionEvent&);
	void start(Entity);
	void update(Entity);
};

