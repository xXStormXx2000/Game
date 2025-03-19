#include "Scene.h"

CompMap<Sprite>& Scene::getSprites() {
	return this->sprites;
}

void Scene::setEntitys(std::unordered_set<Entity>& newEntitys, CompMap<EntityFlags>& newEntitysFlags) {
	assert(newEntitys.size() == newEntitysFlags.size() && "Entity flags mismatch");
	this->entitys = std::move(newEntitys);
	this->entitysFlags = std::move(newEntitysFlags);
}

std::unordered_set<Entity>& Scene::getEntitys() {
	return this->entitys;
}

void Scene::addEntity(Entity entity, EntityFlags entityFlags)
{
	this->entitys.insert(entity);
	this->entitysFlags[entity.getId()] = entityFlags;
}

void Scene::removeEntity(Entity entity) {
	if (this->entitysFlags[entity.getId()].haveComponent(TransformFlag)) this->transforms.erase(entity.getId());
	if (this->entitysFlags[entity.getId()].haveComponent(ColliderFlag)) this->colliders.erase(entity.getId());
	if (this->entitysFlags[entity.getId()].haveComponent(RigidbodyFlag)) this->rigidbodys.erase(entity.getId());
	if (this->entitysFlags[entity.getId()].haveComponent(SpriteFlag)) this->sprites.erase(entity.getId());

	this->entitys.erase(entity);
	this->entitysFlags.erase(entity.getId());
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
