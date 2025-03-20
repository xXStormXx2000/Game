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
	using CollisionMap = std::unordered_map<int, std::unordered_set<int>>;
	using ZoneMap = std::unordered_map<int, DynamicArray<int>>;

	Scene* scene;

	DynamicArray<System*> *systems;

	SharedResources* sharedResources;

	DynamicArray<Entity> collisionEntitys;
	DynamicArray<Entity> dynamicCollisionEntitys;
	SystemFunctionMap customCollisionResolve;

	DynamicArray<Entity> physicsEntitys;

	CompMap* tfMap = nullptr;
	CompMap* efMap = nullptr;
	CompMap* clMap = nullptr;

	std::unordered_map<int, CollisionEvent> entityMinTimesCollisionEventX;
	std::unordered_map<int, CollisionEvent> entityMinTimesCollisionEventY;

	long long minArea = 0;


	void checkForCollision(Entity, CollisionMap&);

	void generateCollisionZones(ZoneMap&, Vector3D, float, float, DynamicArray<Entity>&, DynamicArray<DynamicArray<Entity>>&);

	void preventIntersection(const CollisionEvent&);

	void velocityAdjustCollisionBox(Transform&, const Collider&);

	Transform& getTfComponent(int);
	EntityFlags& getEfComponent(int);
	Collider& getClComponent(int);

	
public: 

	CollisionMap generateCollisionMap();

	void addCustomCollisionResolve(int, System*);

	void setScene(Scene*);
	void setSystems(DynamicArray<System*>*);
	void setSharedResources(SharedResources*);

	CollisionEventMap getAllCollisions(CollisionMap);
	void resolveCollision(const CollisionEventMap&);

	void setCollisionEntitys(DynamicArray<Entity>&);
	void setDynamicCollisionEntitys(DynamicArray<Entity>&);

	void applyVelocity(int);

	static bool simpleCollisionCheck(Vector3D, Vector3D, Vector3D, Vector3D);

	static bool raycast(Vector3D, Vector3D, Vector3D, Vector3D);
};

