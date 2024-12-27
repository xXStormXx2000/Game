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

	DynamicArray<CollisionEvent> checkForCollision(Entity, CollisionMap&, const CompMap<Transform>&, const CompMap<Collider>&, const CompMap<EntityFlags>&) const;

	void generateCollisionZones(ZoneMap&, Vector3D, float, float, DynamicArray<Entity>&, DynamicArray<DynamicArray<Entity>>&, const CompMap<Transform>&, const CompMap<Collider>&, const CompMap<EntityFlags>&);
	CollisionMap generateCollisionMap(const CompMap<Transform>&, const CompMap<Collider>&, const CompMap<EntityFlags>& efMap);

	void preventIntersection(const CollisionEvent&);

	void velocityAdjustCollisionBox(Transform&, const Collider&);


public: 

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

