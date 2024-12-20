#include "FileManager.h"

void FileManager::loadScene(std::string path, DynamicArray<System*>& systems) {

    assert(path[path.size() - 4] == '.' && path[path.size() - 3] == 'w' && path[path.size() - 2] == 'g' && path[path.size() - 1] == 'f'
        && "Non-matching file format");

    DynamicArray<DynamicArray<Entity>> systemsEntitys(systems.size());

    DynamicArray<Entity> entitys;
    CompMap<EntityFlags> entitysFlags;

    CompMap<Transform> transforms;
    CompMap<Collider> colliders;
    CompMap<Rigidbody> rigidbodys;
    CompMap<Sprite> sprites;

    this->renderer->destroyTextures();

    DrawMap entitysToRender;
    DynamicArray<Entity> collisionEntitys;
    DynamicArray<Entity> dynamicCollisionEntitys;
    
    std::ifstream file(path);
    assert(!file.bad());

    unsigned int width, height;

    file >> width >> height;
    this->scene->setWidth(width);
    this->scene->setHeight(height);


    std::string str;
    while (file >> str) {
        if (str == "!") {
            entitys.pushBack(this->createEntity());
            EntityFlags ef;
            file >> ef.flags >> ef.components;
            entitysFlags[entitys.back().getId()] = ef;
            continue;
        }

        if (str == "sys") {
            int num;
            file >> num;
            systemsEntitys[num].pushBack(entitys.back());
            continue;
        }

        if (str == "Transform") {
            Transform tf;
            file >> tf.position.x >> tf.position.y >> tf.position.z;
            file >> tf.velocity.x >> tf.velocity.y >> tf.velocity.z;
            //file >> tf.rotation.x >> tf.rotation.y >> tf.rotation.z;
            file >> tf.scale.x >> tf.scale.y >> tf.scale.z;
            transforms[entitys.back().getId()] = tf;
            continue;
        }

        if (str == "Collider") {
            Collider cl;
            file >> cl.Offset.x >> cl.Offset.y >> cl.Offset.z;
            file >> cl.width >> cl.height;
            colliders[entitys.back().getId()] = cl;
            continue;
        }

        if (str == "Rigidbody") {
            Rigidbody rb;
            file >> rb.mass >> rb.density >> rb.friction >> rb.restitution;
            file >> rb.centerOfMass.x >> rb.centerOfMass.y >> rb.centerOfMass.z;
            file >> rb.acceleration.x >> rb.acceleration.y >> rb.acceleration.z;
            //file >> rb.angularVelocity.x >> rb.angularVelocity.y >> rb.angularVelocity.z;
            rigidbodys[entitys.back().getId()] = rb;
            continue;
        }

        if (str == "Sprite") {
            Sprite sr;
            file >> sr.Offset.x >> sr.Offset.y >> sr.Offset.z;
            file >> sr.width >> sr.height;
            file >> sr.texturePortion.x >> sr.texturePortion.y >> sr.texturePortion.w >> sr.texturePortion.h;
            file >> sr.spriteIndex;
            sprites[entitys.back().getId()] = sr;
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
            entitysToRender[transforms[entitys.back().getId()].position.z].pushBack(entitys.back());
        }

        if (str == "Collision") {
            if (entitysFlags[entitys.back().getId()].getFlag(Dynamic)) {
                dynamicCollisionEntitys.pushBack(entitys.back());
            }
            collisionEntitys.pushBack(entitys.back());
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
