#include "System.h"

void System::setSharedResources(SharedResources* sh){
	this->sharedResources = sh;
}

void System::setScene(Scene* scene) {
	this->scene = scene;
}

void System::setEntitys(DynamicArray<Entity>& entitys) {
	this->entitys = std::move(entitys);
}

void System::run(void (System::*function)(Entity)) {
	for (Entity entity : entitys) {
		EntityFlags flags = this->scene->getComponent<EntityFlags>(entity.getId());
		if (flags.getActive()) (this->*function)(entity);
	}
}

DynamicArray<Entity>& System::getEntitys() {
	return this->entitys;
}

double System::getDeltaTime() {
	return this->sharedResources->getDeltaTime();
}

bool System::keyDown(char key) {
	return this->sharedResources->getKeyPressed(key);
}

void System::onCollision(const CollisionEvent& colEvent) {
}

void System::start(Entity) {
}

void System::preUpdate(Entity) {
}

void System::update(Entity) {
}

void System::postUpdate(Entity) {
}

void System::end(Entity) {
}
