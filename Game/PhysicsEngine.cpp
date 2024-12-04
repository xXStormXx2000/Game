#include "PhysicsEngine.h"

DynamicArray<CollisionEvent> PhysicsEngine::CheckForCollision(Entity entity, int current) {
    Entity nearestXEntity = entity, nearestYEntity = entity;
    double nearestXTime = 1, nearestYTime = 1;

    DynamicArray<CollisionEvent> collisionEvents;

    // Retrieve components for the current entity
    Transform tf = this->scene->getComponent<Transform>(entity.getId());
    Collider cl = this->scene->getComponent<Collider>(entity.getId());

    for (int j = 0; j < this->collisionEntitys.size(); j++) {
        if (j == current) continue;
        Entity otherEntity = this->collisionEntitys[j];

        // Retrieve components for the other entity
        Transform otherTf = this->scene->getComponent<Transform>(otherEntity.getId());
        Collider otherCl = this->scene->getComponent<Collider>(otherEntity.getId());

        // Calculate relative position and velocity
        Vector3D relativePosition = otherTf.position + otherCl.Offset - (tf.position + cl.Offset);
        Vector3D relativeVelocity = (otherTf.velocity - tf.velocity)*this->sharedResources->getDeltaTime();
        // Calculate collision times for X and Y axes
        auto [xEnter, xExit] = this->calculateCollisionTime(relativePosition.x, relativeVelocity.x, cl.width*tf.scale.x, otherCl.width*otherTf.scale.x);
        auto [yEnter, yExit] = this->calculateCollisionTime(relativePosition.y, relativeVelocity.y, cl.height*tf.scale.y, otherCl.height * otherTf.scale.y);

        // Check for overlapping collision intervals
        if (xEnter <= yExit && yEnter <= xExit) {
            if (xEnter <= yEnter && nearestYTime > yEnter && yEnter >= -1) {
                nearestYTime = yEnter;
                nearestYEntity = otherEntity;
                float dir = float((relativeVelocity.y > 0) - (relativeVelocity.y < 0));
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestYEntity, { 0, -dir, 0 }, nearestYTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestYEntity, entity, { 0, dir, 0 }, nearestYTime));
            }
            if (xEnter > yEnter && nearestXTime > xEnter && xEnter >= -1) {
                nearestXTime = xEnter;
                nearestXEntity = otherEntity;
                float dir = float((relativeVelocity.x > 0) - (relativeVelocity.x < 0));
                collisionEvents.pushBack(this->createCollisionEvent(entity, nearestXEntity, { -dir, 0, 0 }, nearestXTime));
                collisionEvents.pushBack(this->createCollisionEvent(nearestXEntity, entity, { dir, 0, 0 }, nearestXTime)); 
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
    EntityFlags otherEf = this->scene->getComponent<EntityFlags>(id);

    if (ef.getSolid() && otherEf.getSolid()) {
        if (colEvent.collisionDirection.x) { // X-direction
            double move = tf.velocity.x * colEvent.time * this->sharedResources->getDeltaTime();
            tf.position.x += move;
            ef.setMovedX(true);
        }
        if (colEvent.collisionDirection.y) { // Y-direction
            double move = tf.velocity.y * colEvent.time * this->sharedResources->getDeltaTime();
            tf.position.y += move;
            ef.setMovedY(true);
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

PhysicsEngine::PhysicsEngine(Scene* scene, DynamicArray<System*>* sys, SharedResources* sR): scene(scene), systems(sys), sharedResources(sR){
}

CollisionEventMap PhysicsEngine::checkAndResolveAllCollisions() {
    CollisionEventMap collisionEvents;

    for (int i = 0; i < this->collisionEntitys.size(); i++) {

        // Check for collisions
        DynamicArray<CollisionEvent> colEvents = CheckForCollision(this->collisionEntitys[i], i);

        // Handle X-direction collision
        
        for (const CollisionEvent& colEvent: colEvents) {
            resolveCollision(colEvent);
            collisionEvents[colEvent.entity.getId()].pushBack(colEvent);
        }
    }

    return collisionEvents;
}

void PhysicsEngine::setCollisionEntitys(DynamicArray<Entity>& entitys) {
    this->collisionEntitys = std::move(entitys);
}

void PhysicsEngine::applyVelocity(int id){
    Transform& tf = this->scene->getComponent<Transform>(id);
    EntityFlags& ef = this->scene->getComponent<EntityFlags>(id);
    Vector3D move = tf.velocity * this->sharedResources->getDeltaTime();
    if (!ef.getMovedX()) tf.position.x += move.x;
    if (!ef.getMovedY()) tf.position.y += move.y;
    ef.setMovedX(true);
    ef.setMovedY(true);
}

bool PhysicsEngine::simpleCollisionCheck(int id, double x, double y) {
    Transform tf = this->scene->getComponent<Transform>(id);
    Collider cl = this->scene->getComponent<Collider>(id);
    for (int j = 0; j < this->collisionEntitys.size(); j++) if(this->collisionEntitys[j].getId() !=  id) {
        int otherId = this->collisionEntitys[j].getId();
        Transform otherTf = this->scene->getComponent<Transform>(otherId);
        Collider otherCl = this->scene->getComponent<Collider>(otherId);
        if (tf.position.x + x < otherTf.position.x + otherCl.width && tf.position.x + cl.width + x > otherTf.position.x &&
            tf.position.y + y < otherTf.position.y + otherCl.height && tf.position.y + cl.height + y > otherTf.position.y) return true;
    }
    return false;
}
