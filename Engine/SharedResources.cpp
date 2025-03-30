#include "SharedResources.h"

void SharedResources::updateMouseState() {
    this->mouseButtonBitmask = SDL_GetMouseState(&this->mouseX, &this->mouseY);
}

Vector3D SharedResources::getMousePos() {
    return { float(this->mouseX), float(this->mouseY), 0 };
}

bool SharedResources::leftMouseButton() {
    return this->mouseButtonBitmask & 0b1;
}

bool SharedResources::middleMouseButton() {
    return this->mouseButtonBitmask & 0b10;
}

bool SharedResources::rightMouseButton() {
    return this->mouseButtonBitmask & 0b100;
}

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

const std::filesystem::path& SharedResources::getSceneFilePath() {
    return this->sceneFilePath;
}

void SharedResources::setSceneFilePath(const std::filesystem::path& path) {
    this->sceneFilePath = path;
}

bool SharedResources::getSceneChange() {
    return this->sceneChange;
}

void SharedResources::setSceneChange(bool b){
    this->sceneChange = b;
}

void SharedResources::addEntityTag(const std::string& tag, Entity entity) {
    this->tagEntityLookup[tag].insert(entity);
}

void SharedResources::removeEntityTag(const std::string& tag, Entity entity) {
    if (this->tagEntityLookup.find(tag) == this->tagEntityLookup.end()) return;
    this->tagEntityLookup[tag].erase(entity);
    if (this->tagEntityLookup[tag].size()) return;
    this->tagEntityLookup.erase(tag);
}

const std::unordered_set<Entity>& SharedResources::getTagEntitys(const std::string& tag) {
    if (this->tagEntityLookup.find(tag) == this->tagEntityLookup.end()) return {};
    return this->tagEntityLookup[tag];
}

void SharedResources::setWindowSize(int w, int h) {
    this->windowWidth = w;
    this->windowHeight = h;
}

int SharedResources::getWindowWidth() {
    return this->windowWidth;
}

int SharedResources::getWindowHeight() {
    return this->windowHeight;
}

std::string SharedResources::getTextInput() {
    return this->textInput;
}

void SharedResources::concatenateTextInput(const std::string& str) {
    this->textInput += str;
}

void SharedResources::setTextInput(const std::string& str) {
    this->textInput = str;
}

void SharedResources::popBackTextInput() {
    if (this->textInput.size() == 0) return;
    this->textInput.pop_back();
}

void SharedResources::toggleTextInputState() {
    if (this->textInputSate) this->textInput = "";
    this->textInputSate = !this->textInputSate;
}

bool SharedResources::getTextInputState() {
    return this->textInputSate;
}
