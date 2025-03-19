#pragma once
#include "Entity.h"
#include "Scene.h"
#include <unordered_set>
#include <thread>
#include <string>
#include "SharedResources.h"
#include "CollisionEvent.h"

class System {
	SharedResources* sharedResources;
	DynamicArray<System*>* systems;
	Scene* scene;
	std::unordered_set<Entity> entitys;
protected:
	float getDeltaTime();

	template<class T>
	T& getComponent(int entityId) {
		return this->scene->getComponent<T>(entityId);
	};
	
	bool keyPressed(char);
	bool keyDown(char);
	bool keyReleased(char);

	void addEntityToSystem(Entity, int);
	void removeEntityFromSystem(Entity, int);

public:
	void setSharedResources(SharedResources*);
	void setScene(Scene*);
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

