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
	using CollisionMap = std::unordered_map<int, DynamicArray<Entity>>;

	Scene* scene;

	DynamicArray<System*> *systems;

	SharedResources* sharedResources;

	DynamicArray<Entity> collisionEntitys;
	DynamicArray<Entity> dynamicCollisionEntitys;
	systemFunctionMap customCollisionResolve;

	DynamicArray<Entity> physicsEntitys;

	DynamicArray<CollisionEvent> checkForCollision(Entity, CollisionMap&);
	std::pair<double, double> calculateCollisionTime(double, double, double, double);
	void resolveCollision(const CollisionEvent&);
	CollisionEvent createCollisionEvent(Entity, Entity, Vector3D, double);

	CollisionMap generateCollisionMap();


public: 
	void addCustomCollisionResolve(int, System*);

	void setScene(Scene*);
	void setSystems(DynamicArray<System*>*);
	void setSharedResources(SharedResources*);


	CollisionEventMap checkAndResolveAllCollisions();

	void setCollisionEntitys(DynamicArray<Entity>&);
	void setDynamicCollisionEntitys(DynamicArray<Entity>&);

	void applyVelocity(int);

	bool simpleCollisionCheck(Vector3D, Vector3D, Vector3D, Vector3D);
};

