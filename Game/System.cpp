#include "System.h"

void System::addEntityToRenderer(Entity entity) {
	this->renderer->addEntity(entity);
}

void System::removeEntityToRenderer(Entity entity) {
	this->renderer->removeEntity(entity);
}

void System::addEntityToPhysicsEngine(Entity entity) {
	this->newPhysicsEntity.pushBack(entity);
}

std::string System::getScene() {
	return this->sharedResources->getSceneFilePath();
}

void System::changeScene(std::string path) {
	this->sharedResources->setSceneFilePath(path);
	this->sharedResources->setSceneChange(true);
}

void System::setSharedResources(SharedResources* sh){
	this->sharedResources = sh;
}

void System::setScene(Scene* scene) {
	this->scene = scene;
}

void System::setRenderer(Renderer* r) {
	this->renderer = r;
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

Entity System::createEntity() {
	return this->scene->createEntity();
}

void System::addEntity(Entity entity, EntityFlags* ef) {
	this->scene->addEntity(entity, ef);
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
	this->systems->at(sys)->getEntitys().insert(entity);
}

void System::removeEntityFromSystem(Entity entity, int sys) {
	this->systems->at(sys)->getEntitys().erase(entity);
}

void System::onCollision(const CollisionEvent& colEvent) {
	Transform& tf = this->getComponent<Transform>(colEvent.entity.getId());

	if (colEvent.collisionDirection.x) {
		tf.velocity.x = 0;
	}
	if (colEvent.collisionDirection.y) {
		tf.velocity.y = 0;
	}
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
