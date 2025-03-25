#pragma once
#include "Scene.h"
#include "DynamicArray.h"
#include <unordered_map>
#include <map>
#include <unordered_set>
#include "System.h"
#include "CollisionEvent.h"
#include "System.h"
#include "SharedResources.h"

class PhysicsEngine {
	using SystemFunctionMap = std::unordered_map<int, DynamicArray<System*>>;
	using CollisionMap = std::unordered_map<int, DynamicArray<int>>;
	using ZoneMap = std::unordered_map<int, DynamicArray<int>>;

	Scene* scene;

	DynamicArray<System*> *systems;

	SharedResources* sharedResources;

	std::unordered_set<Entity> collisionEntitys;
	std::unordered_set<Entity> dynamicCollisionEntitys;
	SystemFunctionMap customCollisionResolve;

	DynamicArray<Entity> physicsEntitys;

	CompMap* tfMap = nullptr;
	CompMap* efMap = nullptr;
	CompMap* clMap = nullptr;

	CollisionEvent createCollisionEvent(Entity entity, Entity other, Vector3D dir, float collisionTime);

	long long minArea;

	DynamicArray<CollisionEvent> checkForCollision(int, const DynamicArray<int>&);

	void generateCollisionZones(Vector3D, float, float, DynamicArray<std::pair<int, Transform>>&, DynamicArray<DynamicArray<std::pair<int, Transform>>>&);

	void preventIntersection(const CollisionEvent&);

	CollisionMap generateCollisionMap(DynamicArray<std::pair<int, Transform>>&);

	DynamicArray<std::pair<int, Transform>> velocityAdjustCollisionBoxes();

	Transform* getTfComponent(int);
	EntityFlags* getEfComponent(int);
	Collider* getClComponent(int);

	void calculateMinArea();
	
public: 
	void addAndRemoveEntitys();

	void addCustomCollisionResolve(int, System*);

	void setScene(Scene*);
	void setSystems(DynamicArray<System*>*);
	void setSharedResources(SharedResources*);

	CollisionEventMap getAllCollisions();
	void resolveCollision(const CollisionEventMap&);

	void setCollisionEntitys(DynamicArray<Entity>&);
	void setDynamicCollisionEntitys(DynamicArray<Entity>&);

	void applyVelocity(int);

	static bool simpleCollisionCheck(Vector3D, Vector3D, Vector3D, Vector3D);

	static bool raycast(Vector3D, Vector3D, Vector3D, Vector3D);
};

