#include "PhysicsEngine.h"

DynamicArray<CollisionEvent> PhysicsEngine::checkForCollision(Entity entity, CollisionMap& possibleCollisions) {
    Entity nearestXEntity = entity, nearestYEntity = entity;
    double nearestXTime = 1, nearestYTime = 1;

    DynamicArray<CollisionEvent> collisionEvents;

    // Retrieve components for the current entity
    Transform tf = this->scene->getComponent<Transform>(entity.getId());
    Collider cl = this->scene->getComponent<Collider>(entity.getId());
    EntityFlags ef = this->scene->getComponent<EntityFlags>(entity.getId());
    DynamicArray<Entity> pc = possibleCollisions[entity.getId()];
    for (Entity otherEntity: pc) {

        // Retrieve components for the other entity
        Transform otherTf = this->scene->getComponent<Transform>(otherEntity.getId());
        Collider otherCl = this->scene->getComponent<Collider>(otherEntity.getId());

        // Calculate relative position and velocity
        Vector3D relativePosition = otherTf.position + otherCl.Offset - (tf.position + cl.Offset);
        Vector3D relativeVelocity = (otherTf.velocity - tf.velocity)*this->sharedResources->getDeltaTime();
        // Calculate collision times for X and Y axes
        auto [xEnter, xExit] = this->calculateCollisionTime(relativePosition.x, relativeVelocity.x, cl.width*tf.scale.x, otherCl.width*otherTf.scale.x);
        auto [yEnter, yExit] = this->calculateCollisionTime(relativePosition.y, relativeVelocity.y, cl.height*tf.scale.y, otherCl.height*otherTf.scale.y);

        // Check for overlapping collision intervals
        if (xEnter < yExit && yEnter < xExit) {
            if (yEnter < xEnter && xEnter >= 0 && nearestXTime > xEnter && !ef.getFlag(MovedX)) {
                float dir = float((relativePosition.x > 0) - (relativePosition.x < 0));
                //if(xEnter < 0) debugMessage(xEnter);
                nearestXTime = xEnter;
                nearestXEntity = otherEntity;
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestXEntity, { -dir, 0, 0 }, nearestXTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestXEntity, entity, { dir, 0, 0 }, nearestXTime));
            }

            if (xEnter < yEnter && yEnter >= 0 && nearestYTime > yEnter && !ef.getFlag(MovedY)) {
                float dir = float((relativePosition.y > 0) - (relativePosition.y < 0));
                //if (yEnter < 0) debugMessage(yEnter);
                nearestYTime = yEnter;
                nearestYEntity = otherEntity;
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestYEntity, { 0, -dir, 0 }, nearestYTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestYEntity, entity, { 0, dir, 0 }, nearestYTime));
            }
        }
    }

    return collisionEvents;
}

std::pair<double, double> PhysicsEngine::calculateCollisionTime(double position, double velocity, double sizeA, double sizeB) {
    if (velocity == 0) {
        double timeEnter = std::numeric_limits<double>::infinity();
        if (position > -sizeB and position < sizeA) timeEnter *= -1;
        return { timeEnter, std::numeric_limits<double>::infinity() };
    }
    double timeEnter = (sizeA - position) / velocity;
    double timeExit = -(position + sizeB) / velocity;

    return { std::min(timeEnter, timeExit), std::max(timeEnter, timeExit) };
}

CollisionEvent PhysicsEngine::createCollisionEvent(Entity entity, Entity other, Vector3D velocity, double collisionTime) {
    CollisionEvent cEvent;

    if (other.getId() != entity.getId()) {
        cEvent.entity = entity;
        cEvent.other = other;
        cEvent.time = collisionTime;
        cEvent.collisionDirection = velocity;
    }

    return cEvent;
}

PhysicsEngine::CollisionMap PhysicsEngine::generateCollisionMap() {
    CollisionMap possibleCollisions;
    for (Entity entity : this->dynamicCollisionEntitys) {
        Transform tf = this->scene->getComponent<Transform>(entity.getId());
        Collider cl = this->scene->getComponent<Collider>(entity.getId());

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
            Transform otherTf = this->scene->getComponent<Transform>(other.getId());
            Collider otherCl = this->scene->getComponent<Collider>(other.getId());

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

void PhysicsEngine::resolveCollision(const CollisionEvent& colEvent) {
    // Resolve collision along the appropriate axis
    int id = colEvent.entity.getId();
    
    Transform& tf = this->scene->getComponent<Transform>(id);
    EntityFlags& ef = this->scene->getComponent<EntityFlags>(id);
    if (!ef.getFlag(Dynamic)) return;

    int otherId = colEvent.other.getId();
    Transform otherTf = this->scene->getComponent<Transform>(otherId);
    EntityFlags otherEf = this->scene->getComponent<EntityFlags>(otherId);

    if (ef.getFlag(Solid) && otherEf.getFlag(Solid)) {
        int dirX = colEvent.collisionDirection.x;
        int dirY = colEvent.collisionDirection.y;
        if (otherEf.getFlag(Dynamic)) {
            if (dirX) { // X-direction
                double move = tf.velocity.x * colEvent.time * this->sharedResources->getDeltaTime();
                tf.position.x += move;
                ef.setFlag(MovedX, true);
            }
            if (dirY) { // Y-direction
                double move = tf.velocity.y * colEvent.time * this->sharedResources->getDeltaTime();
                tf.position.y += move;
                ef.setFlag(MovedY, true);
            }
        } else {
            Collider cl = this->scene->getComponent<Collider>(id);
            Collider otherCl = this->scene->getComponent<Collider>(otherId);
            if (dirX < 0) {
                tf.position.x = otherTf.position.x - cl.width*tf.scale.x;
                ef.setFlag(MovedX, true);
            }
            if (dirX > 0) {
                tf.position.x = otherTf.position.x + otherCl.width*otherTf.scale.x;
                ef.setFlag(MovedX, true);
            }
            if (dirY < 0) {
                tf.position.y = otherTf.position.y - cl.height * tf.scale.y;
                ef.setFlag(MovedY, true);
            }
            if (dirY > 0) {
                tf.position.y = otherTf.position.y + otherCl.height * otherTf.scale.y;
                ef.setFlag(MovedY, true);
            }
        }
    } 

    DynamicArray<System*> systems = customCollisionResolve[id];
    for (System* sys : systems) {
        sys->onCollision(colEvent);
    }
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

CollisionEventMap PhysicsEngine::checkAndResolveAllCollisions() {
    CollisionMap possibleCollisions = generateCollisionMap();

    CollisionEventMap collisionEvents;
    bool collision = true;
    for (int i = 0; i < this->dynamicCollisionEntitys.size() && collision; i++) {
        collision = false;
        DynamicArray<CollisionEvent> colEvents = { CollisionEvent(), CollisionEvent(), CollisionEvent(), CollisionEvent() };

        ///////////////Can me multiple without causing issues////////////////
        for (Entity entity : this->dynamicCollisionEntitys) {
            // Check for collisions
            DynamicArray<CollisionEvent> colEventsTemp = checkForCollision(entity, possibleCollisions);

            for (int k = 0; k < colEventsTemp.size(); k += 2) {
                if (colEventsTemp[k].collisionDirection.x &&
                    colEventsTemp[k].time < colEvents[0].time) {
                    colEvents[0] = colEventsTemp[k];
                    colEvents[1] = colEventsTemp[k + 1];
                }
                if (colEventsTemp[k].collisionDirection.y &&
                    colEventsTemp[k].time < colEvents[2].time) {
                    colEvents[2] = colEventsTemp[k];
                    colEvents[3] = colEventsTemp[k + 1];
                }
            }
        }
        //////////////////////// end ///////////////////////////
        for (const CollisionEvent& colEvent : colEvents) {
            if (colEvent.entity.getId() < 0) continue;
            resolveCollision(colEvent);
            collisionEvents[colEvent.entity.getId()].pushBack(colEvent);
            collision = true;
        }
    }

    return collisionEvents;
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