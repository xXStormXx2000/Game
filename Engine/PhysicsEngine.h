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
	using SystemFunctionMap = std::unordered_map<Entity, DynamicArray<System*>>;
	using CollisionMap = std::unordered_map<Entity, DynamicArray<Entity>>;
	using ZoneMap = std::unordered_map<Entity, DynamicArray<Entity>>;

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

	DynamicArray<CollisionEvent> checkForCollision(Entity, const DynamicArray<Entity>&);

	void generateCollisionZones(Vector3D, float, float, DynamicArray<std::pair<Entity, Transform>>&, DynamicArray<DynamicArray<std::pair<Entity, Transform>>>&);

	void preventIntersection(const CollisionEvent&);

	CollisionMap generateCollisionMap(DynamicArray<std::pair<Entity, Transform>>&);

	DynamicArray<std::pair<Entity, Transform>> velocityAdjustCollisionBoxes();

	Transform* getTfComponent(Entity);
	EntityFlags* getEfComponent(Entity);
	Collider* getClComponent(Entity);

	void calculateMinArea();
	
public: 
	void addAndRemoveEntitys();

	void addCustomCollisionResolve(Entity, System*);

	void setScene(Scene*);
	void setSystems(DynamicArray<System*>*);
	void setSharedResources(SharedResources*);

	CollisionEventMap getAllCollisions();
	void resolveCollision(const CollisionEventMap&);

	void setCollisionEntitys(DynamicArray<Entity>&);
	void setDynamicCollisionEntitys(DynamicArray<Entity>&);

	void applyVelocity(Entity);

	static bool simpleCollisionCheck(Vector3D, Vector3D, Vector3D, Vector3D);

	static bool raycast(Vector3D, Vector3D, Vector3D, Vector3D);
};

