#pragma once
#include "Entity.h"
#include "DynamicArray.h"
#include <unordered_map>
#include <unordered_set>
#include <typeinfo>
#include <typeindex>
#include "Components.h"
#include "Debug.h"

using CompMap = std::unordered_map<Entity, Component*>;
using CompMapList = std::unordered_map <std::type_index, CompMap>;

class Scene {
	std::unordered_set<Entity> entitys;
	CompMapList components;


	unsigned int width;
	unsigned int height;

	int entityCount = 0;

public:
	template<class T>
	CompMap& getComponents() {
		auto it = components.find(typeid(T));
		if (it != components.end()) {
			return it->second;
		}
		assert(false && "No such component");
	}

	void setEntitys(std::unordered_set<Entity>& newEntitys, CompMap& entitysFlags);
	std::unordered_set<Entity>& getEntitys();

	void addEntity(Entity, EntityFlags*);
	void removeEntity(Entity);

	void setWidth(unsigned int);
	void setHeight(unsigned int);

	unsigned int getWidth();
	unsigned int getHeight();

	void setComponents(CompMapList&);

	template<class T>
	T& getComponent(Entity entity) {
		assert(this->getComponents<T>().find(entity) != this->getComponents<T>().end() && "Entity has no such component");
		return *(dynamic_cast<T*>(this->getComponents<T>()[entity]));
	}

	template<class T>
	void addComponent(Entity entity, T* component) {
		this->getComponents<T>()[entity] = dynamic_cast<Component*>(component);
	}

	template<class T>
	void removeComponent(Entity entity) {
		this->getComponents<T>().erase(entity);
	}

	template<class T>
	bool haveComponent(Entity entity) {
		return (this->getComponents<T>().find(entity) != this->getComponents<T>().end());
	}

	Entity createEntity();
};
