#pragma once
#include "System.h"
#include "DynamicArray.h"
#include "unordered_map"
#include "spring.h"
#include "Debug.h"
#include "somData.h"

class SoftbodyNode : public System {
public:
	void onCollision(const CollisionEvent&);
	void update(Entity);
};

