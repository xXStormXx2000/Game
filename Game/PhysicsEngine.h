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
	DynamicArray<Entity> staticCollisionEntitys;
	DynamicArray<Entity> physicsEntitys;
	systemFunctionMap customCollisionResolve;


	DynamicArray<CollisionEvent> checkForCollision(Entity, int);
	std::pair<double, double> calculateCollisionTime(double, double, double, double);
	void resolveCollision(const CollisionEvent&);
	CollisionEvent createCollisionEvent(Entity, Entity, Vector3D, double);


public:
	void addCustomCollisionResolve(int, System*);

	void setScene(Scene*);
	void setSystems(DynamicArray<System*>*);
	void setSharedResources(SharedResources*);


	CollisionEventMap checkAndResolveAllCollisions();

	void setCollisionEntitys(DynamicArray<Entity>&);
	void setStaticCollisionEntitys(DynamicArray<Entity>&);

	void applyVelocity(int);

	bool simpleCollisionCheck(int, double, double, int, double, double);
};

