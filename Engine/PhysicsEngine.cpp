#include "PhysicsEngine.h"

CollisionEvent PhysicsEngine::createCollisionEvent(Entity entity, Entity other, Vector3D dir, float collisionTime)
{
    CollisionEvent cEvent;
    if (other.getId() == entity.getId()) return cEvent;

    cEvent.entity = entity;
    cEvent.other = other;
    cEvent.time = collisionTime;
    cEvent.collisionDirection = dir;

    return cEvent;
}

DynamicArray<CollisionEvent> PhysicsEngine::checkForCollision(Entity entity, const DynamicArray<Entity>& possibleCollisions) {
    CollisionEvent collisionEventX;
    CollisionEvent collisionEventY;
    
    // Retrieve components for the current entity
    Transform tf = *getTfComponent(entity);

    EntityFlags ef = *getEfComponent(entity);

    if (ef.getFlag(MovedX)) tf.velocity.x = 0;
    if (ef.getFlag(MovedY)) tf.velocity.y = 0;

    if (!(tf.velocity * tf.velocity)) return {};

    const Collider* cl = getClComponent(entity);
    tf.position += cl->offset;

    for (int i = 0; i < possibleCollisions.size(); i++) {
        Entity otherEntity = possibleCollisions.at(i);
        // Retrieve components for the other entity
        Transform otherTf = *getTfComponent(otherEntity);
        EntityFlags otherEf = *getEfComponent(otherEntity);

        if (otherEf.getFlag(MovedX)) otherTf.velocity.x = 0;
        if (otherEf.getFlag(MovedY)) otherTf.velocity.y = 0;


        // Calculate relative velocity
        Vector3D relativeVelocity = -tf.velocity;
        if (otherEf.getFlag(Dynamic)) relativeVelocity += otherTf.velocity;

        if (!(relativeVelocity * relativeVelocity)) continue;

        // Calculate relative position
        const Collider* otherCl = getClComponent(otherEntity);
        otherTf.position += otherCl->offset;

        Vector3D relativePosition = otherTf.position - tf.position;
        const float smallNum = 0.001;
        // Calculate collision times for X and Y axes
        auto calculateCollisionTime = [smallNum](float position, float velocity, float sizeA, float sizeB) -> std::pair<float, float> {
            if (abs(velocity) < smallNum) {
                float timeEnter = std::numeric_limits<float>::infinity();
                if (position > -sizeB && position < sizeA) timeEnter *= -1;
                return { timeEnter, std::numeric_limits<float>::infinity() };
            }
            float timeEnter = -(position - sizeA) / velocity;
            float timeExit = -(position + sizeB) / velocity;
            if (timeEnter > timeExit) return { timeExit, timeEnter };
            return { timeEnter, timeExit };
            };

        auto [xEnter, xExit] = calculateCollisionTime(relativePosition.x, relativeVelocity.x, cl->width * tf.scale.x, otherCl->width * otherTf.scale.x);
        auto [yEnter, yExit] = calculateCollisionTime(relativePosition.y, relativeVelocity.y, cl->height * tf.scale.y, otherCl->height * otherTf.scale.y);

        // Check for overlapping collision intervals
        if (xEnter > yExit || yEnter > xExit) continue;

        if (yEnter < xEnter && -smallNum <= xEnter && xEnter <= 1 + smallNum &&
            collisionEventX.time > xEnter) {

            float dir = float((relativePosition.x >= 0) - (relativePosition.x < 0));
            collisionEventX = createCollisionEvent(entity, otherEntity, { -dir, 0, 0 }, xEnter);
        }

        if (xEnter <= yEnter && -smallNum <= yEnter && yEnter <= 1 + smallNum &&
            collisionEventY.time > yEnter) {

            float dir = float((relativePosition.y >= 0) - (relativePosition.y < 0));
            collisionEventY = createCollisionEvent(entity, otherEntity, { 0, -dir, 0 }, yEnter);
        }
    }
    DynamicArray<CollisionEvent> out;
    if (collisionEventX.time < 2 && !ef.getFlag(MovedX)) out.pushBack(collisionEventX);
    if (collisionEventY.time < 2 && !ef.getFlag(MovedY)) out.pushBack(collisionEventY);
    return out;
}

void PhysicsEngine::generateCollisionZones(Vector3D pos, float width, float height, DynamicArray<std::pair<Entity, Transform>>& entitys, DynamicArray<DynamicArray<std::pair<Entity, Transform>>>& collisionZones) {
    const int minEntitys = 5;
    if (width * height <= this->minArea || entitys.size() <= minEntitys) {
        collisionZones.pushBack(entitys);
        return;
    }
    Vector3D posTwo = pos;
    if (width > height) {
        width /= 2;
        posTwo.x += width;
    } else {
        height /= 2;
        posTwo.y += height;
    }
    DynamicArray<std::pair<Entity, Transform>> entitysOne;
    DynamicArray<std::pair<Entity, Transform>> entitysTwo;
    for (std::pair<Entity, Transform>& entity : entitys) {
        Transform& tf = entity.second;
        if (simpleCollisionCheck(pos, { width, height, 0}, tf.position, tf.velocity)) {
            entitysOne.pushBack(entity);
        }
        if (simpleCollisionCheck(posTwo, { width, height, 0 }, tf.position, tf.velocity)) {
            entitysTwo.pushBack(entity);
        }
    }
    generateCollisionZones(pos, width, height, entitysOne, collisionZones);
    generateCollisionZones(posTwo, width, height, entitysTwo, collisionZones);
}

PhysicsEngine::CollisionMap PhysicsEngine::generateCollisionMap(DynamicArray<std::pair<Entity, Transform>>& aCollisionBoxes) {
    DynamicArray<DynamicArray<std::pair<Entity, Transform>>> collisionZones(1);
    float width = this->scene->getWidth();
    float height = this->scene->getHeight();
    for (std::pair<Entity, Transform>& entity : aCollisionBoxes) {
        Transform& tf = entity.second;
        if (0 > tf.position.x || width < tf.position.x + tf.velocity.x ||
            0 > tf.position.y || height < tf.position.y + tf.velocity.y) {
            collisionZones[0].pushBack(entity);
        }
    }
    generateCollisionZones({ 0, 0, 0 }, width, height, aCollisionBoxes, collisionZones);
    std::unordered_map<Entity, DynamicArray<int>> lookup;
    for (int i = 0; i < collisionZones.size(); i++) {
        for (auto& [entity, tf] : collisionZones[i]) {
            lookup[entity].pushBack(i);
        }
    }
    CollisionMap collisionMap;
    for (auto& [entity, tf] : aCollisionBoxes) {
        for (int zone : lookup[entity]) {
            for (auto& [otherEntity, otherTf] : collisionZones[zone]) {
                if (entity == otherEntity) continue;
                if (!simpleCollisionCheck(tf.position, tf.velocity, otherTf.position, otherTf.velocity)) continue;
                collisionMap[entity].pushBack(otherEntity);
            }
        }
    }
    return collisionMap;
}

void PhysicsEngine::preventIntersection(const CollisionEvent& colEvent) {
    // Resolve collision along the appropriate axis
    Entity entity = colEvent.entity;
    EntityFlags* ef = getEfComponent(entity);
    if (!ef->getFlag(Dynamic)) return;

    Transform* tf = getTfComponent(entity);

    Entity otherEntity = colEvent.other;
    Transform otherTf = *getTfComponent(otherEntity);
    EntityFlags otherEf = *getEfComponent(otherEntity);

    if (!ef->getFlag(Solid) || !otherEf.getFlag(Solid)) return;

    Collider cl = *getClComponent(entity);
    Collider otherCl = *getClComponent(otherEntity);

    auto helper = [&](Flags moved, int dir, float& pos, float vel, float size, float otherPos, float otherSize) -> void {
        if (!dir) return;
        if (otherEf.getFlag(Dynamic) && !otherEf.getFlag(moved)) {
            float move = vel * colEvent.time;
            pos += move;
            ef->setFlag(moved, true);
        }
        else {
            if (dir < 0) {
                pos = otherPos - size;
                ef->setFlag(moved, true);
            }
            if (dir > 0) {
                pos = otherPos + otherSize;
                ef->setFlag(moved, true);
            }
        }
        };

    if (!ef->getFlag(MovedX))
        helper(MovedX, colEvent.collisionDirection.x, tf->position.x, tf->velocity.x, cl.width * tf->scale.x, otherTf.position.x, otherCl.width * otherTf.scale.x);
    if (!ef->getFlag(MovedY))
        helper(MovedY, colEvent.collisionDirection.y, tf->position.y, tf->velocity.y, cl.height * tf->scale.y, otherTf.position.y, otherCl.height * otherTf.scale.y);
}

CollisionEventMap PhysicsEngine::getAllCollisions() {

    DynamicArray<std::pair<Entity,Transform>> aCollisionBoxes = velocityAdjustCollisionBoxes();
    calculateMinArea();
    CollisionMap possibleCollisions = generateCollisionMap(aCollisionBoxes);
    if (possibleCollisions.size() == 0) return {};
    using Event = std::tuple<float, Entity, Entity, Vector3D>;
    std::map<float, std::unordered_map<Entity, std::unordered_map<Entity, Vector3D>>> collisionQueue;
    std::unordered_map<Entity, std::unordered_set<float>> lookup;

    for (Entity entity : this->collisionEntitys) {
        if (possibleCollisions.find(entity.getId()) == possibleCollisions.end()) continue;
        DynamicArray<CollisionEvent> entityCols = checkForCollision(entity, possibleCollisions.at(entity));
        for (CollisionEvent& colEvent : entityCols) {
            collisionQueue[colEvent.time][entity.getId()][colEvent.other.getId()] = colEvent.collisionDirection;
            lookup[entity.getId()].insert(colEvent.time);
        }
    }
    CollisionEventMap out;
    while (collisionQueue.size() != 0) {
        float time = collisionQueue.begin()->first;
        std::unordered_set<Entity> entitys;
        std::unordered_map<Entity, std::unordered_map<Entity, Vector3D>>& collisions = collisionQueue.begin()->second;

        for (auto& collisionEvents : collisions) {
            for (auto& c : collisionEvents.second) {
                CollisionEvent collisionEvent = createCollisionEvent(collisionEvents.first, c.first, c.second, time);
                preventIntersection(collisionEvent);
                out[collisionEvent.entity.getId()].pushBack(collisionEvent);
                entitys.insert(collisionEvent.entity.getId());
            }
        }
        collisionQueue.erase(time);

        auto helper = [&](Entity entity) {
            for (float t : lookup[entity]) {
                if (collisionQueue.find(t) == collisionQueue.end()) continue;
                collisionQueue[t].erase(entity);
            }
            lookup.erase(entity);
            DynamicArray<CollisionEvent> entityCols = checkForCollision(entity, possibleCollisions.at(entity));
            for (CollisionEvent& colEvent : entityCols) {
                collisionQueue[colEvent.time][entity][colEvent.other.getId()] = colEvent.collisionDirection;
                lookup[entity].insert(colEvent.time);
            }
        };
        for (Entity entity : entitys) {
            helper(entity);
            for (Entity otherEntity : possibleCollisions[entity]) {
                helper(otherEntity);
            }
        }
    }
    return out;
}

DynamicArray<std::pair<Entity, Transform>> PhysicsEngine::velocityAdjustCollisionBoxes() {
    DynamicArray<std::pair<Entity, Transform>> collisionBoxes;
    for (Entity entity : this->collisionEntitys) {
        Transform tf = *this->getTfComponent(entity.getId());
        const Collider* cl = this->getClComponent(entity.getId());
        tf.position += cl->offset;

        if (tf.velocity.x < 0) {
            tf.position.x += tf.velocity.x;
            tf.velocity.x *= -1;
        }
        tf.velocity.x += cl->width * tf.scale.x;

        if (tf.velocity.y < 0) {
            tf.position.y += tf.velocity.y;
            tf.velocity.y *= -1;
        }
        tf.velocity.y += cl->height * tf.scale.y;
        collisionBoxes.pushBack({ entity.getId(), tf });
    }
    return collisionBoxes;
}

Transform* PhysicsEngine::getTfComponent(Entity entityId) {
	return dynamic_cast<Transform*>(this->tfMap->at(entityId));
}

EntityFlags* PhysicsEngine::getEfComponent(Entity entityId) {
	return dynamic_cast<EntityFlags*>(this->efMap->at(entityId));
}

Collider* PhysicsEngine::getClComponent(Entity entityId) {
	return dynamic_cast<Collider*>(this->clMap->at(entityId));
}

void PhysicsEngine::calculateMinArea() {
    if (this->dynamicCollisionEntitys.size() == 0) {
        this->minArea = 5;
        return;
    }
    this->minArea = 0;
    for (Entity entity: this->dynamicCollisionEntitys) {
        const Transform* tf = this->getTfComponent(entity.getId());
        const Collider* cl = this->getClComponent(entity.getId());
        this->minArea += (tf->scale.x*cl->width + abs(tf->velocity.x))*(tf->scale.y*cl->height + abs(tf->velocity.y));
    }
    this->minArea /= this->dynamicCollisionEntitys.size();
    this->minArea *= 5; // Magic number to optimize zone size
    if (this->minArea < 5) this->minArea = 5;
}

void PhysicsEngine::addAndRemoveEntitys() {
    for (System* sys : *this->systems) {
        for (Entity entity : sys->newPhysicsEntity) {
            if (this->scene->getComponent<EntityFlags>(entity.getId()).getFlag(Dynamic)) {
                this->dynamicCollisionEntitys.insert(entity);
            }
            this->collisionEntitys.insert(entity);
            for (System* sys : *this->systems) {
                const std::unordered_set<Entity>& e = sys->getEntitys();
                if(e.find(entity) != e.end()) addCustomCollisionResolve(entity.getId(), sys);
            }
        }
        sys->newPhysicsEntity.empty();
        for (Entity entity : sys->newPhysicsEntity) {
            this->collisionEntitys.erase(entity);
            this->dynamicCollisionEntitys.erase(entity);
        }
        sys->oldPhysicsEntity.empty();
    }
}

void PhysicsEngine::addCustomCollisionResolve(Entity entity, System* ptr) {
    this->customCollisionResolve[entity].pushBack(ptr);
}

void PhysicsEngine::setScene(Scene* scene) {
    this->scene = scene;
	this->tfMap = &this->scene->getComponents<Transform>();
	this->efMap = &this->scene->getComponents<EntityFlags>();
	this->clMap = &this->scene->getComponents<Collider>();
}

void PhysicsEngine::setSystems(DynamicArray<System*>* sys){
    this->systems = sys;
}

void PhysicsEngine::setSharedResources(SharedResources* sh) {
    this->sharedResources = sh;
}

void PhysicsEngine::resolveCollision(const CollisionEventMap& collisionsMap) {
    for (const auto&  [entity, col] : collisionsMap) {
        DynamicArray<System*>& systems = this->customCollisionResolve[entity];
        for (int i = 0; i < col.size(); i++) {
            const CollisionEvent& collision = col.at(i);
            for (System* sys : systems) sys->onCollision(collision);
        }
    }
}

void PhysicsEngine::setCollisionEntitys(DynamicArray<Entity>& entitys) {
    for(Entity entity: entitys) this->collisionEntitys.insert(entity);
}

void PhysicsEngine::setDynamicCollisionEntitys(DynamicArray<Entity>& entitys) {
    for (Entity entity : entitys) this->dynamicCollisionEntitys.insert(entity);
}

void PhysicsEngine::applyVelocity(Entity entity){
    Transform* tf = getTfComponent(entity);
    EntityFlags* ef = getEfComponent(entity);
    Vector3D move = tf->velocity;
    if (!ef->getFlag(MovedX)) tf->position.x += move.x;
    if (!ef->getFlag(MovedY)) tf->position.y += move.y;
    ef->setFlag(MovedX, true);
    ef->setFlag(MovedY, true);
}

bool PhysicsEngine::simpleCollisionCheck(Vector3D pos, Vector3D rect, Vector3D otherPos, Vector3D otherRect) {
    return (pos.x <= otherPos.x + otherRect.x && pos.x + rect.x >= otherPos.x &&
        pos.y <= otherPos.y + otherRect.y && pos.y + rect.y >= otherPos.y);
}

bool PhysicsEngine::raycast(Vector3D pos1, Vector3D vec1, Vector3D pos2, Vector3D vec2) {
    float t1 = ((pos2 - pos1).crossProduct(vec2).z) / (vec1.crossProduct(vec2).z);
    float t2 = ((pos1 - pos2).crossProduct(vec1).z) / (vec2.crossProduct(vec1).z);
    return 0 <= t1 && t1 <= 1 && 0 <= t2 && t2 <= 1;
}
