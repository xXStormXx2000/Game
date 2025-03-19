#include "FileManager.h"

void FileManager::loadScene(std::string path, DynamicArray<System*>& systems) {

    assert(path[path.size() - 4] == '.' && path[path.size() - 3] == 'w' && path[path.size() - 2] == 'g' && path[path.size() - 1] == 'f'
        && "Non-matching file format");

	DynamicArray<std::unordered_set<Entity>> systemsEntitys(systems.size()); // Entitys for each system

	std::unordered_set<Entity> entitys; // All entitys
	CompMap<EntityFlags> entitysFlags; // Flags for each entity

	CompMap<Transform> transforms; // Transform component for each entity
	CompMap<Collider> colliders;   // Collider component for each entity
	CompMap<Rigidbody> rigidbodys; // Rigidbody component for each entity
	CompMap<Sprite> sprites;       // Sprite component for each entity

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
            EntityFlags ef;
            file >> ef.flags >> ef.components;
            entitysFlags[entity.getId()] = ef;
            continue;
        }

        if (str == "sys") {
            int num;
            file >> num;
            systemsEntitys[num].insert(entity);
            continue;
        }

        if (str == "Transform") {
            Transform tf;
            file >> tf.position.x >> tf.position.y >> tf.position.z;
            file >> tf.velocity.x >> tf.velocity.y >> tf.velocity.z;
            //file >> tf.rotation.x >> tf.rotation.y >> tf.rotation.z;
            file >> tf.scale.x >> tf.scale.y >> tf.scale.z;
            transforms[entity.getId()] = tf;
            continue;
        }

        if (str == "Collider") {
            Collider cl;
            file >> cl.offset.x >> cl.offset.y >> cl.offset.z;
            file >> cl.width >> cl.height;
            colliders[entity.getId()] = cl;
            continue;
        }

        if (str == "Rigidbody") {
            Rigidbody rb;
            file >> rb.mass >> rb.density >> rb.friction >> rb.restitution;
            file >> rb.centerOfMass.x >> rb.centerOfMass.y >> rb.centerOfMass.z;
            file >> rb.acceleration.x >> rb.acceleration.y >> rb.acceleration.z;
            //file >> rb.angularVelocity.x >> rb.angularVelocity.y >> rb.angularVelocity.z;
            rigidbodys[entity.getId()] = rb;
            continue;
        }

        if (str == "Sprite") {
            Sprite sr;
            file >> sr.offset.x >> sr.offset.y >> sr.offset.z;
            file >> sr.width >> sr.height;
            file >> sr.texturePortion.x >> sr.texturePortion.y >> sr.texturePortion.w >> sr.texturePortion.h;
            file >> sr.spriteIndex;
            sprites[entity.getId()] = sr;
            continue;
        }

        if (str == "Textures") {
            file >> str;
            while (str != "#") {
                this->renderer->createTexture(str);
                file >> str;
            }
            continue;
        }

        if (str == "Render") {
            entitysToRender[transforms[entity.getId()].position.z].pushBack(entity);
        }

        if (str == "Collision") {
            if (entitysFlags[entity.getId()].getFlag(Dynamic)) {
                dynamicCollisionEntitys.pushBack(entity);
            }
            collisionEntitys.pushBack(entity);
        }
    }

    file.close();

    for (int i = 0; i < systems.size(); i++) systems[i]->setEntitys(systemsEntitys[i]);

    this->scene->setEntitys(entitys, entitysFlags);

    this->scene->setComponents(transforms);
    this->scene->setComponents(colliders);
    this->scene->setComponents(rigidbodys);
    this->scene->setComponents(sprites);

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
