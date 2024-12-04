#include "SharedResources.h"

DynamicArray<CollisionEvent> SharedResources::getCollisionEvents(int id) {
    return this->collisionEvents[id];
}

void SharedResources::setCollisionEvents(CollisionEventMap newCollisionEvents) {
    this->collisionEvents = std::move(newCollisionEvents);
}

void SharedResources::resetKeysPressed() {
    for (int i = 0; i < 32; i++) {
        this->keysPressed[i] = 0;
    }
}

bool SharedResources::getKeyPressed(char key) {
    return this->keysPressed[key / 8] & 1 << (key%8);
}

void SharedResources::setKeyPressed(char key, bool input) {
    this->keysPressed[key / 8] &= ~(1 << (key % 8));
    this->keysPressed[key / 8] |= input << (key % 8);
}

void SharedResources::setDeltaTime(double dt) {
    this->deltaTime = dt;
}

double SharedResources::getDeltaTime() {
    return this->deltaTime;
}
