#pragma once
#include "Scene.h"
#include "DynamicArray.h"
#include "unordered_map"
#include "System.h"
#include "CollisionEvent.h"
#include "System.h"
#include "SharedResources.h"

#include <thread>
#include <mutex>



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

	DynamicArray<CollisionEvent> checkForCollision(Entity, CollisionMap&, const CompMap<Transform>&, const CompMap<Collider>&, const CompMap<EntityFlags>&) const;
	std::pair<float, float> calculateCollisionTime(float, float, float, float) const;
	
	CollisionEvent createCollisionEvent(Entity, Entity, Vector3D, float) const;

	CollisionMap generateCollisionMap(const CompMap<Transform>&, const CompMap<Collider>&);

	void preventIntersection(const CollisionEvent&);


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

	bool simpleCollisionCheck(Vector3D, Vector3D, Vector3D, Vector3D);
};

