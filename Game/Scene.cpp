#include "Scene.h"

CompMap<Sprite>& Scene::getSprites() {
	return this->sprites;
}

void Scene::setEntitys(DynamicArray<Entity>& newEntitys, CompMap<EntityFlags>& newEntitysFlags) {
	assert(newEntitys.size() == newEntitysFlags.size() && "Entity flags mismatch");
	this->entitys = std::move(newEntitys);
	this->entitysFlags = std::move(newEntitysFlags);
}

DynamicArray<Entity>& Scene::getEntitys() {
	return this->entitys;
}
