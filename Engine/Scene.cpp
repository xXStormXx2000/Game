#include "Scene.h"

void Scene::setEntitys(std::unordered_set<Entity>& newEntitys, CompMap& newEntitysFlags) {
	assert(newEntitys.size() == newEntitysFlags.size() && "Entity flags mismatch");
	this->entitys = std::move(newEntitys);
	this->components[typeid(EntityFlags)] = std::move(newEntitysFlags);
}

std::unordered_set<Entity>& Scene::getEntitys() {
	return this->entitys;
}

void Scene::addEntity(Entity entity, EntityFlags* entityFlags)
{
	this->entitys.insert(entity);
	this->components[typeid(EntityFlags)][entity.getId()] = dynamic_cast<Component*>(entityFlags);
}

void Scene::removeEntity(Entity entity) {
	for (auto& components : this->components) {
		components.second.erase(entity.getId());
	}

	this->entitys.erase(entity);
}

void Scene::setWidth(unsigned int w) {
	this->width = w;
}

void Scene::setHeight(unsigned int h) {
	this->height = h;
}

unsigned int Scene::getWidth() {
	return this->width;
}

unsigned int Scene::getHeight() {
	return this->height;
}

void Scene::setComponents(CompMapList& newComponents) {
	this->components = std::move(newComponents);
}

Entity Scene::createEntity() {
	Entity entity(this->entityCount);
	this->entityCount++;
	return entity;
}
