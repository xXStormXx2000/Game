#pragma once
#include "Scene.h"
#include "DynamicArray.h"
#include "unordered_map"
#include "System.h"
#include "CollisionEvent.h"
#include "System.h"
#include "SharedResources.h"

class PhysicsEngine {
	using systemFunctionMap = std::unordered_map<int, DynamicArray<System*>>;

	Scene* scene;

	DynamicArray<System*> *systems;

	SharedResources* sharedResources;

	DynamicArray<Entity> collisionEntitys;
	DynamicArray<Entity> physicsEntitys;
	systemFunctionMap customCollisionResolve;


	DynamicArray<CollisionEvent> CheckForCollision(Entity, int);
	std::pair<double, double> calculateCollisionTime(double, double, double, double);
	void resolveCollision(const CollisionEvent&);
	CollisionEvent createCollisionEvent(Entity, Entity, Vector3D, double);


public:
	void addCustomCollisionResolve(int, System*);
	PhysicsEngine(Scene* = nullptr, DynamicArray<System*>* = nullptr, SharedResources* = nullptr);
	CollisionEventMap checkAndResolveAllCollisions();
	void setCollisionEntitys(DynamicArray<Entity>&);
	void applyVelocity(int);
	bool simpleCollisionCheck(int, double, double);
};

