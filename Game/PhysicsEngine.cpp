#include "PhysicsEngine.h"

DynamicArray<CollisionEvent> PhysicsEngine::checkForCollision(Entity entity, int current) {
    Entity nearestXEntity = entity, nearestYEntity = entity;
    double nearestXTime = 1, nearestYTime = 1;

    DynamicArray<CollisionEvent> collisionEvents;

    // Retrieve components for the current entity
    Transform tf = this->scene->getComponent<Transform>(entity.getId());
    Collider cl = this->scene->getComponent<Collider>(entity.getId());

    const DynamicArray<Entity>* list = &this->collisionEntitys;
    if(current == -1) list = &this->staticCollisionEntitys;

    for (int j = 0; j < list->size(); j++) {
        if (j == current) continue;
        Entity otherEntity = list->at(j);

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
            if (yEnter <= xEnter && xEnter >= 0 && nearestXTime > xEnter) {
                float dir = float((relativeVelocity.x > 0) - (relativeVelocity.x < 0));
                nearestXTime = xEnter;
                nearestXEntity = otherEntity;
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestXEntity, { dir, 0, 0 }, nearestXTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestXEntity, entity, { -dir, 0, 0 }, nearestXTime));
            }

            if (xEnter < yEnter && yEnter >= 0 && nearestYTime > yEnter) {
                float dir = float((relativeVelocity.y > 0) - (relativeVelocity.y < 0));
                nearestYTime = yEnter;
                nearestYEntity = otherEntity;
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestYEntity, { 0, dir, 0 }, nearestYTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestYEntity, entity, { 0, -dir, 0 }, nearestYTime));
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

void PhysicsEngine::resolveCollision(const CollisionEvent& colEvent) {
    // Resolve collision along the appropriate axis
    int id = colEvent.entity.getId();
    
    Transform& tf = this->scene->getComponent<Transform>(id);
    EntityFlags& ef = this->scene->getComponent<EntityFlags>(id);
    if (ef.getFlag(Static)) return;

    int otherId = colEvent.other.getId();
    Transform otherTf = this->scene->getComponent<Transform>(otherId);
    EntityFlags otherEf = this->scene->getComponent<EntityFlags>(otherId);

    if (ef.getFlag(Solid) && otherEf.getFlag(Solid)) {
        int dirX = colEvent.collisionDirection.x;
        int dirY = colEvent.collisionDirection.y;
        if (!otherEf.getFlag(Static)) {
            if (dirX && !ef.getFlag(MovedX)) { // X-direction
                double move = tf.velocity.x * colEvent.time * this->sharedResources->getDeltaTime();
                tf.position.x += move;
                ef.setFlag(MovedX, true);
            }
            if (dirY && !ef.getFlag(MovedY)) { // Y-direction
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
    CollisionEventMap collisionEvents;
    for (int i = 0; i < this->collisionEntitys.size(); i++) {
        // Check for collisions
        DynamicArray<CollisionEvent> colEvents = checkForCollision(this->collisionEntitys[i], -1);

        for (const CollisionEvent& colEvent : colEvents) {
            resolveCollision(colEvent);
            collisionEvents[colEvent.entity.getId()].pushBack(colEvent);
        }
    }
    for (int i = 0; i < this->collisionEntitys.size(); i++) {
        // Check for collisions
        DynamicArray<CollisionEvent> colEvents = checkForCollision(this->collisionEntitys[i], i);
        
        for (const CollisionEvent& colEvent : colEvents) {
            resolveCollision(colEvent);
            collisionEvents[colEvent.entity.getId()].pushBack(colEvent);
        }
    }

    return collisionEvents;
}

void PhysicsEngine::setCollisionEntitys(DynamicArray<Entity>& entitys) {
    this->collisionEntitys = std::move(entitys);
}

void PhysicsEngine::setStaticCollisionEntitys(DynamicArray<Entity>& entitys) {
    this->staticCollisionEntitys = std::move(entitys);
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

bool PhysicsEngine::simpleCollisionCheck(int id, double x, double y, int otherId, double otherX, double otherY) {
    Collider cl = this->scene->getComponent<Collider>(id);
    Collider otherCl = this->scene->getComponent<Collider>(otherId);
    if (x < otherX + otherCl.width && x + cl.width > otherX &&
        y < otherY + otherCl.height && y + cl.height > otherY) return true;
    return false;
}