#pragma once
#include "Entity.h"
#include "Scene.h"
#include <unordered_set>
#include "DynamicArray.h"
#include <string>
#include "SharedResources.h"
#include "CollisionEvent.h"
#include "Renderer.h"

class System {
	SharedResources* sharedResources;
	DynamicArray<System*>* systems;
	Scene* scene;
	Renderer* renderer;
	std::unordered_set<Entity> entitys;
protected:
	float getDeltaTime();

	Entity createEntity();

	void addEntity(Entity, EntityFlags*);

	template<class T>
	T& getComponent(int entityId) {
		return this->scene->getComponent<T>(entityId);
	};
	
	bool keyPressed(char);
	bool keyDown(char);
	bool keyReleased(char);

	void addEntityToSystem(Entity, int);
	void removeEntityFromSystem(Entity, int);

	template<class T>
	void addComponentToEntity(Entity entity, T* comp) {
		this->scene->addComponent(entity.getId(), comp);
	}

	template<class T>
	void removeComponentToEntity(Entity entity) {
		this->scene->removeComponent<T>(entity.getId());
	}

	void addEntityToRenderer(Entity);
	void removeEntityToRenderer(Entity);

	void addEntityToPhysicsEngine(Entity);
	void removeEntityToPhysicsEngine(Entity);

public:
	DynamicArray<Entity> newPhysicsEntity;
	void setSharedResources(SharedResources*);
	void setScene(Scene*);
	void setRenderer(Renderer*);

	void run(void (System::*)(Entity));

	void setSystems(DynamicArray<System*>* systems);

	void setEntitys(std::unordered_set<Entity>& entitys);

	std::unordered_set<Entity>& getEntitys();
	

	virtual void onCollision(const CollisionEvent&);
	virtual void start(Entity);

	virtual void preUpdate(Entity);
	virtual void update(Entity);
	virtual void postUpdate(Entity);


	virtual void end(Entity);

	virtual ~System() {};
};

