#include "Entity.h"

Entity::Entity(int id): id(id) {
}

int Entity::getId() const {
	return this->id;
}

bool Entity::operator==(const Entity& other) const {
	return this->id == other.id;
}
