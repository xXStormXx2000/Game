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
