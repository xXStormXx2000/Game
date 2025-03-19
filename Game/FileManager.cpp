#include "FileManager.h"

void FileManager::loadScene(std::string path, DynamicArray<System*>& systems, const DynamicArray<Component*>& componentsTypes) {

    assert(path[path.size() - 4] == '.' && path[path.size() - 3] == 'w' && path[path.size() - 2] == 'g' && path[path.size() - 1] == 'f'
        && "Non-matching file format");

	DynamicArray<std::unordered_set<Entity>> systemsEntitys(systems.size()); // Entitys for each system

	std::unordered_set<Entity> entitys; // All entitys
	CompMap entitysFlags;               // Flags for each entity

	CompMapList components;           // Components

	this->renderer->destroyTextures(); // Clear all textures

	DrawMap entitysToRender;                      // Entitys to render
	DynamicArray<Entity> collisionEntitys;        // Entitys that can collide
	DynamicArray<Entity> dynamicCollisionEntitys; // Entitys that can collide and are dynamic
    
    std::ifstream file(path);
    assert(!file.bad());

    unsigned int width, height;

    file >> width >> height;
    this->scene->setWidth(width);
    this->scene->setHeight(height);

    Entity entity;
    std::string str;
    while (file >> str) {
        if (str == "!") {
			entity = this->createEntity();
            entitys.insert(entity);
            EntityFlags* ef = new EntityFlags;
            file >> ef->flags;
            entitysFlags[entity.getId()] = dynamic_cast<Component*>(ef);
            continue;
        }

        if (str == "sys") {
            int num;
            file >> num;
            systemsEntitys[num].insert(entity);
            continue;
        }
        for (int i = 0; i < componentsTypes.size(); i++) {
            Component* comp = componentsTypes.at(i)->read_file(file, str);
			if (comp == nullptr) continue;
			components[typeid(*componentsTypes.at(i))][entity.getId()] = comp;
        }
        if (str == "Render") {
			// not good code need to be redone
            entitysToRender[dynamic_cast<Transform*>(components[typeid(Transform)][entity.getId()])->position.z].pushBack(entity);
        }
        if (str == "Textures") {
            file >> str;
            while (str != "#") {
                this->renderer->createTexture(str);
                file >> str;
            }
            continue;
        }

        if (str == "Collision") {
            if (dynamic_cast<EntityFlags*>(entitysFlags[entity.getId()])->getFlag(Dynamic)) {
                dynamicCollisionEntitys.pushBack(entity);
            }
            collisionEntitys.pushBack(entity);
        }
    }

    file.close();

    for (int i = 0; i < systems.size(); i++) systems[i]->setEntitys(systemsEntitys[i]);

    this->scene->setComponents(components);
    this->scene->setEntitys(entitys, entitysFlags);

    this->renderer->setEntitys(entitysToRender);

    this->physicsEngine->setCollisionEntitys(collisionEntitys);
    this->physicsEngine->setDynamicCollisionEntitys(dynamicCollisionEntitys);
}

Entity FileManager::createEntity() {
    Entity entity(this->entityCount);
    this->entityCount++;
    return entity;
}

void FileManager::setScene(Scene* scene) {
    this->scene = scene;
}

void FileManager::setRenderer(Renderer* renderer) {
    this->renderer = renderer;
}

void FileManager::setPhysicsEngine(PhysicsEngine* physicsEngine) {
    this->physicsEngine = physicsEngine;
}

void FileManager::setAudioManager(AudioManager* audioManager) {
    this->audioManager = audioManager;
}
