#pragma once
#include "Entity.h"
#include "DynamicArray.h"
#include <unordered_map>
#include <unordered_set>
#include <typeinfo>
#include <typeindex>
#include "Components.h"
#include "Debug.h"

using CompMap = std::unordered_map<int, Component*>;
using CompMapList = std::unordered_map <std::type_index, std::unordered_map<int, Component*>>;

class Scene {
	std::unordered_set<Entity> entitys;
	CompMapList components;


	unsigned int width;
	unsigned int height;


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
	T* getComponent(int entityId) {
		assert(this->getComponents<T>().find(entityId) != this->getComponents<T>().end() && "Entity has no such component");
		return dynamic_cast<T*>(this->getComponents<T>()[entityId]);
	}

	template<class T>
	void addComponent(int entityId, T* component) {
		this->getComponents<T>()[entityId] = dynamic_cast<Component*>(component);
	}

	template<class T>
	void removeComponent(int entityId) {
		this->getComponents<T>().erase(entityId);
	}
};
