#include "FileManager.h"

void FileManager::loadScene(const std::filesystem::path& path, DynamicArray<System*>& systems, DynamicArray<Component*>& componentsTypes) {

    assert(path.extension() == ".wgf" && "Non-matching file format");

	DynamicArray<std::unordered_set<Entity>> systemsEntitys(systems.size()); // Entitys for each system

	std::unordered_set<Entity> entitys; // All entitys
	CompMap entitysFlags;               // Flags for each entity

	CompMapList components;           // Components
    for (Component* c : componentsTypes) components[typeid(*c)];

	this->renderer->destroyTextures(); // Clear all texture

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
			entity = this->scene->createEntity();
            entitys.insert(entity);
            EntityFlags* ef = new EntityFlags;
            file >> ef->flags;
            entitysFlags[entity] = dynamic_cast<Component*>(ef);
            continue;
        }

        if (str == "sys") {
            int num;
            file >> num;
            systemsEntitys[num].insert(entity);
            continue;
        }
        for (int i = 0; i < componentsTypes.size(); i++) {
            Component* comp = componentsTypes.at(i)->readFile(file, str);
			if (comp == nullptr) continue;
			components[typeid(*componentsTypes.at(i))][entity] = comp;
        }
        if (str == "Render") {
			// not good code need to be redone
            this->renderer->addEntity(entity, dynamic_cast<Transform*>(components[typeid(Transform)][entity])->position.z);
        }
        if (str == "Textures") {
            file >> str;
            while (str != "#") {
                this->renderer->createTexture(str);
                file >> str;
            }
            continue;
        }
        if (str == "TileSet") {
            int num;
            file >> num;
            for(int i = 0; i < num; i++) {
                TileSet tileSet;
                tileSet = tileSet.readFile(file);
                this->renderer->addTileSet(tileSet);
            }
            continue;
        }

        if (str == "Collision") {
            if (dynamic_cast<EntityFlags*>(entitysFlags[entity])->getFlag(Dynamic)) {
                dynamicCollisionEntitys.pushBack(entity);
            }
            collisionEntitys.pushBack(entity);
        }
    }

    file.close();

    for (int i = 0; i < systems.size(); i++) systems[i]->setEntitys(systemsEntitys[i]);

    this->scene->setComponents(components);
    this->scene->setEntitys(entitys, entitysFlags);

    this->physicsEngine->setScene(this->scene);
    this->physicsEngine->setCollisionEntitys(collisionEntitys);
    this->physicsEngine->setDynamicCollisionEntitys(dynamicCollisionEntitys);
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
