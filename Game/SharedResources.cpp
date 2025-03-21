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

void SharedResources::setKeyPressed(char key) {
    this->keysPressed[key / 8] |= 1 << (key % 8);
}

bool SharedResources::getKeyDown(char key) {
    return this->keysDown[key / 8] & 1 << (key % 8);
}

void SharedResources::setKeyDown(char key, bool input) {
    this->keysDown[key / 8] &= ~(1 << (key % 8));
    this->keysDown[key / 8] |= input << (key % 8);
}

void SharedResources::resetKeysReleased() {
    for (int i = 0; i < 32; i++) {
        this->keysReleased[i] = 0;
    }
}

bool SharedResources::getKeyReleased(char key) {
    return this->keysReleased[key / 8] & 1 << (key % 8);
}

void SharedResources::setKeyReleased(char key) {
    this->keysReleased[key / 8] |= 1 << (key % 8);
}

void SharedResources::setDeltaTime(float dt) {
    this->deltaTime = dt;
}

float SharedResources::getDeltaTime() {
    return this->deltaTime;
}
