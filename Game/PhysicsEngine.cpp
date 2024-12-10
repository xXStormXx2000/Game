#include "PhysicsEngine.h"

DynamicArray<CollisionEvent> PhysicsEngine::checkForCollision(Entity entity, CollisionMap& possibleCollisions, const CompMap<Transform>& tfMap, const CompMap<Collider>& clMap, const CompMap<EntityFlags>& efMap) const {
    

    Entity nearestXEntity = entity, nearestYEntity = entity;
    float nearestXTime = 1, nearestYTime = 1;

    DynamicArray<CollisionEvent> collisionEvents;
    // Retrieve components for the current entity
    Transform tf = tfMap.at(entity.getId());
    const Collider& cl = clMap.at(entity.getId());
    EntityFlags ef = efMap.at(entity.getId());

    if (ef.getFlag(MovedX)) tf.velocity.x = 0;
    if (ef.getFlag(MovedY)) tf.velocity.y = 0;

    for (Entity otherEntity: possibleCollisions.at(entity.getId())) {
        // Retrieve components for the other entity
        Transform otherTf = tfMap.at(otherEntity.getId());
        EntityFlags otherEf = efMap.at(otherEntity.getId());

        if (otherEf.getFlag(MovedX)) otherTf.velocity.x = 0;
        if (otherEf.getFlag(MovedY)) otherTf.velocity.y = 0;
        
        // Calculate relative velocity
        Vector3D relativeVelocity = (otherTf.velocity - tf.velocity)*this->sharedResources->getDeltaTime();
        if (!(relativeVelocity.x + relativeVelocity.y)) continue;

        
        // Calculate relative position
        const Collider& otherCl = clMap.at(otherEntity.getId());
        Vector3D relativePosition = otherTf.position + otherCl.Offset - (tf.position + cl.Offset);
        

        // Calculate collision times for X and Y axes
        auto [xEnter, xExit] = this->calculateCollisionTime(relativePosition.x, relativeVelocity.x, cl.width*tf.scale.x, otherCl.width*otherTf.scale.x);
        auto [yEnter, yExit] = this->calculateCollisionTime(relativePosition.y, relativeVelocity.y, cl.height*tf.scale.y, otherCl.height*otherTf.scale.y);

        // Check for overlapping collision intervals
        if (xEnter < yExit && yEnter < xExit) {
            if (yEnter < xEnter && xEnter >= 0 && nearestXTime > xEnter && !ef.getFlag(MovedX)) {
                float dir = float((relativePosition.x > 0) - (relativePosition.x < 0));

                nearestXTime = xEnter;
                nearestXEntity = otherEntity;
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestXEntity, { -dir, 0, 0 }, nearestXTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestXEntity, entity, { dir, 0, 0 }, nearestXTime));
            }

            if (xEnter <= yEnter && yEnter >= 0 && nearestYTime > yEnter && !ef.getFlag(MovedY)) {
                float dir = float((relativePosition.y > 0) - (relativePosition.y < 0));

                nearestYTime = yEnter;
                nearestYEntity = otherEntity;
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestYEntity, { 0, -dir, 0 }, nearestYTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestYEntity, entity, { 0, dir, 0 }, nearestYTime));
            }
        }
    }
    return collisionEvents;
}

std::pair<float, float> PhysicsEngine::calculateCollisionTime(float position, float velocity, float sizeA, float sizeB) const {
    if (velocity == 0) {
        float timeEnter = std::numeric_limits<float>::infinity();
        if (position > -sizeB and position < sizeA) timeEnter *= -1;
        return { timeEnter, std::numeric_limits<float>::infinity() };
    }
    float timeEnter = (sizeA - position) / velocity;
    float timeExit = -(position + sizeB) / velocity;
    if (timeEnter > timeExit) return { timeExit, timeEnter };
    return { timeEnter, timeExit };
}

CollisionEvent PhysicsEngine::createCollisionEvent(Entity entity, Entity other, Vector3D velocity, float collisionTime) const {
    CollisionEvent cEvent;

    if (other.getId() != entity.getId()) {
        cEvent.entity = entity;
        cEvent.other = other;
        cEvent.time = collisionTime;
        cEvent.collisionDirection = velocity;
    }

    return cEvent;
}

PhysicsEngine::CollisionMap PhysicsEngine::generateCollisionMap(const CompMap<Transform>& tfMap, const CompMap<Collider>& clMap, const CompMap<EntityFlags>& efMap) {
    CollisionMap possibleCollisions;
    for (Entity entity : this->dynamicCollisionEntitys) {
        Transform tf = tfMap.at(entity.getId());
        if (!(tf.velocity.x + tf.velocity.y) && efMap.at(entity.getId()).getFlag(Solid)) continue;

        const Collider& cl = clMap.at(entity.getId());

        tf.velocity *= this->sharedResources->getDeltaTime();

        if (tf.velocity.x < 0) {
            tf.position.x += tf.velocity.x;
            tf.velocity.x *= -1;
        }
        tf.velocity.x += cl.width * tf.scale.x;

        if (tf.velocity.y < 0) {
            tf.position.y += tf.velocity.y;
            tf.velocity.y *= -1;
        }
        tf.velocity.y += cl.height * tf.scale.y;

        for (Entity other : this->collisionEntitys) {
            if (entity.getId() == other.getId()) continue;
            Transform otherTf = tfMap.at(other.getId());
            Collider otherCl = clMap.at(other.getId());

            otherTf.velocity *= this->sharedResources->getDeltaTime();

            if (otherTf.velocity.x < 0) {
                otherTf.position.x += otherTf.velocity.x;
                otherTf.velocity.x *= -1;
            }
            otherTf.velocity.x += otherCl.width * otherTf.scale.x;

            if (otherTf.velocity.y < 0) {
                otherTf.position.y += otherTf.velocity.y;
                otherTf.velocity.y *= -1;
            }
            otherTf.velocity.y += otherCl.height * otherTf.scale.y;

            if (simpleCollisionCheck(tf.position, tf.velocity, otherTf.position, otherTf.velocity)) {
                possibleCollisions[entity.getId()].pushBack(other);
            }
        }
    }
    return possibleCollisions;
}

void PhysicsEngine::preventIntersection(const CollisionEvent& colEvent) {
    // Resolve collision along the appropriate axis
    int id = colEvent.entity.getId();
    Transform& tf = this->scene->getComponent<Transform>(id);
    EntityFlags& ef = this->scene->getComponent<EntityFlags>(id);
    if(!ef.getFlag(Dynamic)) return;

    int otherId = colEvent.other.getId();
    Transform otherTf = this->scene->getComponent<Transform>(otherId);
    EntityFlags otherEf = this->scene->getComponent<EntityFlags>(otherId);

    if (!ef.getFlag(Solid) || !otherEf.getFlag(Solid)) return;

    Collider cl = this->scene->getComponent<Collider>(id);
    Collider otherCl = this->scene->getComponent<Collider>(otherId);

    auto helper = [&](Flags moved, int dir, float& pos, float vel, float size, float otherPos, float otherSize) -> void {
        if (!dir) return;
        if (otherEf.getFlag(Dynamic) && !otherEf.getFlag(moved)) {
             float move = vel * colEvent.time * this->sharedResources->getDeltaTime();
             pos += move;
             ef.setFlag(moved, true);
        } else {
            if (dir < 0) {
                pos = otherPos - size;
                ef.setFlag(moved, true);
            }
            if (dir > 0) {
                pos = otherPos + otherSize;
                ef.setFlag(moved, true);
            }
        }
    };

    if (!ef.getFlag(MovedX)) 
        helper(MovedX, colEvent.collisionDirection.x, tf.position.x, tf.velocity.x, cl.width * tf.scale.x, otherTf.position.x, otherCl.width*otherTf.scale.x);
    if (!ef.getFlag(MovedY)) 
        helper(MovedY, colEvent.collisionDirection.y, tf.position.y, tf.velocity.y, cl.height * tf.scale.y, otherTf.position.y, otherCl.height * otherTf.scale.y);
}

void PhysicsEngine::addCustomCollisionResolve(int id, System* ptr) {
    this->customCollisionResolve[id].pushBack(ptr);
}

void PhysicsEngine::setScene(Scene* scene) {
    this->scene = scene;
}

void PhysicsEngine::setSystems(DynamicArray<System*>* sys){
    this->systems = sys;
}

void PhysicsEngine::setSharedResources(SharedResources* sh) {
    this->sharedResources = sh;
}

CollisionEventMap PhysicsEngine::getAllCollisions() {
    const CompMap<Transform>& tfMap = this->scene->getComponents<Transform>();
    const CompMap<Collider>& clMap = this->scene->getComponents<Collider>();
    const CompMap<EntityFlags>& efMap = this->scene->getComponents<EntityFlags>();

    CollisionMap possibleCollisions = generateCollisionMap(tfMap, clMap, efMap);
    
    if (!possibleCollisions.size()) return{};
    
    std::map<float, DynamicArray<CollisionEvent>> colEventsMap;

    CollisionEventMap collisionEvents;
    bool collision;
    auto helper = [&](DynamicArray<Entity>& entitys) -> void {
        collision = false;
        for (Entity entity: entitys) {
            if (possibleCollisions.find(entity.getId()) == possibleCollisions.end()) continue;
            EntityFlags ef = efMap.at(entity.getId());
            if (ef.getFlag(MovedX) && ef.getFlag(MovedY)) continue;
            // Check for collisions
            DynamicArray<CollisionEvent> colEventsTemp = checkForCollision(entity, possibleCollisions, tfMap, clMap, efMap);

            for (int k = 0; k < colEventsTemp.size(); k++) {
                int id = colEventsTemp[k].entity.getId();
                if (id < 0) continue;
                colEventsMap[colEventsTemp[k].time].pushBack(colEventsTemp[k]);
                
            }
        }
        if (!colEventsMap.size()) return;
        for (const CollisionEvent& colEvent : colEventsMap.begin()->second) {
            if (colEvent.collisionDirection.x + colEvent.collisionDirection.y == 0) continue;
            if (colEvent.entity.getId() < 0) continue;
            preventIntersection(colEvent);
            collisionEvents[colEvent.entity.getId()].pushBack(colEvent);
            collision = true;
        }
        
    };
    helper(this->dynamicCollisionEntitys);
    for (int i = 1; i < this->dynamicCollisionEntitys.size() && collision; i++) {
        DynamicArray<CollisionEvent> collisions = colEventsMap.begin()->second;
        colEventsMap.erase(colEventsMap.begin()->first);
        std::unordered_set<int> visited;
        DynamicArray<Entity> newCollisions; 

        for (const CollisionEvent& colEvent: collisions) {
            int oldId = colEvent.entity.getId();
            
            if (!efMap.at(oldId).getFlag(Dynamic)) continue;

            for (Entity entity : possibleCollisions[oldId]) {
                int id = entity.getId();
                if (!efMap.at(id).getFlag(Dynamic)) continue;
                if(visited.find(id) != visited.end()) continue;
                visited.insert(id);

                newCollisions.pushBack(entity);
            }

        }
        helper(newCollisions);
    }

    return collisionEvents;
}

void PhysicsEngine::resolveCollision(const CollisionEventMap& collisionsMap) {
    for (const std::pair<int, DynamicArray<CollisionEvent>>& idAndCollisions : collisionsMap) {
        DynamicArray<System*> systems = this->customCollisionResolve[idAndCollisions.first];
        for (int i = 0; i < idAndCollisions.second.size(); i++) {
            const CollisionEvent& collision = idAndCollisions.second.at(i);
            for (System* sys : systems) sys->onCollision(collision);
        }
    }
}

void PhysicsEngine::setCollisionEntitys(DynamicArray<Entity>& entitys) {
    this->collisionEntitys = std::move(entitys);
}

void PhysicsEngine::setDynamicCollisionEntitys(DynamicArray<Entity>& entitys) {
    this->dynamicCollisionEntitys = std::move(entitys);
}

void PhysicsEngine::applyVelocity(int id){
    Transform& tf = this->scene->getComponent<Transform>(id);
    EntityFlags& ef = this->scene->getComponent<EntityFlags>(id);
    Vector3D move = tf.velocity * this->sharedResources->getDeltaTime();
    if (!ef.getFlag(MovedX)) tf.position.x += move.x;
    if (!ef.getFlag(MovedY)) tf.position.y += move.y;
    ef.setFlag(MovedX, true);
    ef.setFlag(MovedY, true);
}

bool PhysicsEngine::simpleCollisionCheck(Vector3D pos, Vector3D rect, Vector3D otherPos, Vector3D otherRect) {
    return (pos.x < otherPos.x + otherRect.x && pos.x + rect.x > otherPos.x &&
        pos.y < otherPos.y + otherRect.y && pos.y + rect.y > otherPos.y);
}