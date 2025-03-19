#pragma once
#include "Entity.h"
#include "DynamicArray.h"
#include <unordered_map>
#include <unordered_set>
#include "Components.h"
#include "Debug.h"

template<class T>
using CompMap = std::unordered_map<int, T>;

class Scene {
	std::unordered_set<Entity> entitys;
	CompMap<EntityFlags> entitysFlags;

	CompMap<Transform> transforms;
	CompMap<Collider> colliders;
	CompMap<Rigidbody> rigidbodys;
	CompMap<Sprite> sprites;


	unsigned int width;
	unsigned int height;


public:

	CompMap<Sprite>& getSprites();

	template<class T>
	CompMap<T>& getComponents() {
		if constexpr (std::is_same_v<T, Transform>) return this->transforms;
		if constexpr (std::is_same_v<T, Collider>) return this->colliders;
		if constexpr (std::is_same_v<T, Rigidbody>) return this->rigidbodys;
		if constexpr (std::is_same_v<T, Sprite>) return this->sprites;
		if constexpr (std::is_same_v<T, EntityFlags>) return this->entitysFlags;
		assert(0 && "Given component do not exist");
	}

	void setEntitys(std::unordered_set<Entity>& newEntitys, CompMap<EntityFlags>& entitysFlags);
	std::unordered_set<Entity>& getEntitys();

	void addEntity(Entity, EntityFlags);
	void removeEntity(Entity);

	void setWidth(unsigned int);
	void setHeight(unsigned int);

	unsigned int getWidth();
	unsigned int getHeight();

	template<class T>
	void setComponents(CompMap<T>& newComponents) {
		this->getComponents<T>() = std::move(newComponents);
	}

	template<class T>
	T& getComponent(int entityId) {
		assert(this->getComponents<T>().find(entityId) != this->getComponents<T>().end() && "Entity has no such component");
		return this->getComponents<T>()[entityId];
	}

	template<class T>
	void addComponent(int entityId, T component) {
		this->getComponents<T>()[entityId] = component;
	}

	template<class T>
	void removeComponent(int entityId) {
		this->getComponents<T>().erase(entityId);
	}
};
