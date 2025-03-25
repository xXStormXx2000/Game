#pragma once
#include "System.h"
#include "somData.h"

class PlayerController: public System {
	const float speed = 7;
	void onCollision(const CollisionEvent&);
	void update(Entity);
};

