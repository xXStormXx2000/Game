#include "System.h"

void System::setSharedResources(SharedResources* sh){
	this->sharedResources = sh;
}

void System::setScene(Scene* scene) {
	this->scene = scene;
}

void System::setSystems(DynamicArray<System*>* systems)
{
	this->systems = systems;
}

void System::setEntitys(std::unordered_set<Entity>& entitys) {
	this->entitys = std::move(entitys);
}

void System::run(void (System::*function)(Entity)) {
	for (Entity entity : entitys) {
		EntityFlags flags = this->scene->getComponent<EntityFlags>(entity.getId());
		if (flags.getFlag(Active)) (this->*function)(entity);
	}
}

std::unordered_set<Entity>& System::getEntitys() {
	return this->entitys;
}

float System::getDeltaTime() {
	return this->sharedResources->getDeltaTime();
}

bool System::keyPressed(char key) {
	return this->sharedResources->getKeyPressed(key);
}

bool System::keyDown(char key) {
	return this->sharedResources->getKeyDown(key);
}

bool System::keyReleased(char key) {
	return this->sharedResources->getKeyReleased(key);
}

void System::addEntityToSystem(Entity entity, int sys) {
	this->systems->operator[](sys)->getEntitys().insert(entity);
}

void System::removeEntityFromSystem(Entity entity, int sys) {
	this->systems->operator[](sys)->getEntitys().erase(entity);
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
