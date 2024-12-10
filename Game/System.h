#pragma once
#include "Entity.h"
#include "Scene.h"
#include "DynamicArray.h"
#include <thread>
#include <string>
#include "SharedResources.h"
#include "CollisionEvent.h"

class System {
	SharedResources* sharedResources;

	Scene* scene;
	DynamicArray<Entity> entitys;
protected:
	float getDeltaTime();

	template<class T>
	T& getComponent(int entityId) {
		return this->scene->getComponent<T>(entityId);
	};
	
	bool keyPressed(char);
	bool keyDown(char);
	bool keyReleased(char);


public:
	void setSharedResources(SharedResources*);
	void setScene(Scene*);
	void run(void (System::*)(Entity));

	void setEntitys(DynamicArray<Entity>& entitys);

	DynamicArray<Entity>& getEntitys();
	

	virtual void onCollision(const CollisionEvent&);
	virtual void start(Entity);

	virtual void preUpdate(Entity);
	virtual void update(Entity);
	virtual void postUpdate(Entity);


	virtual void end(Entity);
};

