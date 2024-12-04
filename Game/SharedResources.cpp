#include "SharedResources.h"

DynamicArray<CollisionEvent> SharedResources::getCollisionEvents(int id) {
    return this->collisionEvents[id];
}

void SharedResources::setCollisionEvents(CollisionEventMap newCollisionEvents) {
    this->collisionEvents = std::move(newCollisionEvents);
}

void SharedResources::setDeltaTime(double dt) {
    this->deltaTime = dt;
}

double SharedResources::getDeltaTime() {
    return this->deltaTime;
}
