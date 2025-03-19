#include "PhysicsEngine.h"

void PhysicsEngine::checkForCollision(Entity entity, CollisionMap& possibleCollisions) {

	CollisionEvent& collisionEventX = this->entityMinTimesCollisionEventX[entity.getId()];
	collisionEventX = CollisionEvent();
	CollisionEvent& collisionEventY = this->entityMinTimesCollisionEventY[entity.getId()];
	collisionEventY = CollisionEvent();

    // Retrieve components for the current entity
    Transform tf = this->tfMap->at(entity.getId());
    
    EntityFlags ef = this->efMap->at(entity.getId());

    if (ef.getFlag(MovedX)) tf.velocity.x = 0;
    if (ef.getFlag(MovedY)) tf.velocity.y = 0;

    if (!(tf.velocity*tf.velocity)) return;
    
    const Collider& cl = this->clMap->at(entity.getId());
    tf.position += cl.offset;

    for (int otherEntity: possibleCollisions.at(entity.getId())) {
        // Retrieve components for the other entity
        Transform otherTf = this->tfMap->at(otherEntity);
        EntityFlags otherEf = this->efMap->at(otherEntity);

        if (otherEf.getFlag(MovedX)) otherTf.velocity.x = 0;
        if (otherEf.getFlag(MovedY)) otherTf.velocity.y = 0;

        
        // Calculate relative velocity
        Vector3D relativeVelocity = -tf.velocity;
        if (otherEf.getFlag(Dynamic)) relativeVelocity += otherTf.velocity;

        if (!(relativeVelocity*relativeVelocity)) continue;

        // Calculate relative position
        const Collider& otherCl = this->clMap->at(otherEntity);
        otherTf.position += otherCl.offset;

        Vector3D relativePosition = otherTf.position - tf.position;

        // Calculate collision times for X and Y axes
        auto calculateCollisionTime = [](float position, float velocity, float sizeA, float sizeB) -> std::pair<float, float> {
            if (abs(velocity) < 0.001) {
                float timeEnter = std::numeric_limits<float>::infinity();
                if (position > -sizeB && position < sizeA) timeEnter *= -1;
                return { timeEnter, std::numeric_limits<float>::infinity() };
            }
            float timeEnter = -(position - sizeA) / velocity;
            float timeExit = -(position + sizeB) / velocity;
            if (timeEnter > timeExit) return { timeExit, timeEnter };
            return { timeEnter, timeExit };
        };

        auto [xEnter, xExit] = calculateCollisionTime(relativePosition.x, relativeVelocity.x, cl.width*tf.scale.x, otherCl.width*otherTf.scale.x);
        auto [yEnter, yExit] = calculateCollisionTime(relativePosition.y, relativeVelocity.y, cl.height*tf.scale.y, otherCl.height*otherTf.scale.y);

        // Check for overlapping collision intervals
        if (xEnter > yExit || yEnter > xExit) continue;

        auto createCollisionEvent = [](Entity entity, Entity other, Vector3D dir, float collisionTime)->CollisionEvent {
            CollisionEvent cEvent;
            if (other.getId() == entity.getId()) return cEvent;

            cEvent.entity = entity;
            cEvent.other = other;
            cEvent.time = collisionTime;
            cEvent.collisionDirection = dir;

            return cEvent;
        };

        if (yEnter < xEnter && 0 <= xEnter && xEnter <= 1 && 
            collisionEventX.time > xEnter) {

            float dir = float((relativePosition.x >= 0) - (relativePosition.x < 0));
            collisionEventX = createCollisionEvent(entity, otherEntity, { -dir, 0, 0 }, xEnter);
        }

        if (xEnter <= yEnter && 0 <= yEnter && yEnter <= 1 && 
            collisionEventY.time > yEnter) {

            float dir = float((relativePosition.y >= 0) - (relativePosition.y < 0));
            collisionEventY = createCollisionEvent(entity, otherEntity, { 0, -dir, 0 }, yEnter);
        }
    }
}

void PhysicsEngine::generateCollisionZones(ZoneMap& zoneMap, Vector3D pos, float width, float height, DynamicArray<Entity>& list, DynamicArray<DynamicArray<Entity>>& collisionZones) {

    if (width*height <= this->minArea || list.size() <= 16) {
        for (Entity entity : list) {
            zoneMap[entity.getId()].pushBack(collisionZones.size());
        }
        collisionZones.pushBack(std::move(list));
        return;
    }
    
    Vector3D offset;
    if (width > height) {
        width /= 2;
        offset.x = width;
    } else {
        height /= 2;
        offset.y = height;
    }

    DynamicArray<Entity> subList1;
    DynamicArray<Entity> subList2;

    for (Entity entity : list) {
        Transform tf = this->tfMap->at(entity.getId());

        const Collider& cl = this->clMap->at(entity.getId());

        velocityAdjustCollisionBox(tf, cl);

        bool box1 = simpleCollisionCheck(tf.position, tf.velocity, pos, { width, height , 0 });
        bool box2 = simpleCollisionCheck(tf.position, tf.velocity, pos + offset, { width, height , 0 });

        if (box1) subList1.pushBack(entity);
        if (box2) subList2.pushBack(entity);
    }
    list.empty();
	list.shrinkToFit();
    generateCollisionZones(zoneMap, pos, width, height, subList1, collisionZones);
    generateCollisionZones(zoneMap, pos + offset, width, height, subList2, collisionZones);
}

PhysicsEngine::CollisionMap PhysicsEngine::generateCollisionMap() {
    float width = this->scene->getWidth(), height = this->scene->getHeight();

    DynamicArray<DynamicArray<Entity>> collisionZones(2);
    ZoneMap zoneMap;

    for (Entity entity : this->collisionEntitys) {
        Transform tf = this->tfMap->at(entity.getId());

        const Collider& cl = this->clMap->at(entity.getId());

        velocityAdjustCollisionBox(tf, cl);
        
        if (this->simpleCollisionCheck(tf.position, tf.velocity, { 0, 0, 0 }, { width, height, 0 })) continue;
        collisionZones[1].pushBack(entity);
        zoneMap[entity.getId()].pushBack(1);
    }
	DynamicArray<Entity> temp = this->collisionEntitys;
    generateCollisionZones(zoneMap, { 0, 0, 0 }, width, height, temp, collisionZones);

    CollisionMap possibleCollisions;
    for (Entity entity: this->dynamicCollisionEntitys) {
        EntityFlags ef = this->efMap->at(entity.getId());
        Transform tf = this->tfMap->at(entity.getId());

        if (!(tf.velocity*tf.velocity) && ef.getFlag(Solid)) continue;

        const Collider& cl = this->clMap->at(entity.getId());
        velocityAdjustCollisionBox(tf, cl);

        DynamicArray<int>& zones = zoneMap.at(entity.getId());
        for (int zone : zones) {
            for (Entity other: collisionZones[zone]) {
                if (entity.getId() == other.getId()) continue;
                Transform otherTf = this->tfMap->at(other.getId());
                const Collider& otherCl = this->clMap->at(other.getId());

                velocityAdjustCollisionBox(otherTf, otherCl);

                if (!simpleCollisionCheck(tf.position, tf.velocity, otherTf.position, otherTf.velocity)) continue;
                possibleCollisions[entity.getId()].insert(other.getId());
                if(this->efMap->at(other.getId()).getFlag(Dynamic)) possibleCollisions[other.getId()].insert(entity.getId());
            }
        }
    }
    return possibleCollisions;
}

void PhysicsEngine::preventIntersection(const CollisionEvent& colEvent) {
    // Resolve collision along the appropriate axis
    int id = colEvent.entity.getId();
    EntityFlags& ef = this->efMap->at(id);
    if(!ef.getFlag(Dynamic)) return;

    Transform& tf = this->tfMap->at(id);

    int otherId = colEvent.other.getId();
    Transform otherTf = this->tfMap->at(otherId);
    EntityFlags otherEf = this->efMap->at(otherId);

    if (!ef.getFlag(Solid) || !otherEf.getFlag(Solid)) return;

    Collider cl = this->clMap->at(id);
    Collider otherCl = this->clMap->at(otherId);

    auto helper = [&](Flags moved, int dir, float& pos, float vel, float size, float otherPos, float otherSize) -> void {
        if (!dir) return;
        if (otherEf.getFlag(Dynamic) && !otherEf.getFlag(moved)) {
            float move = vel * colEvent.time;
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

void PhysicsEngine::velocityAdjustCollisionBox(Transform& tf, const Collider& cl) {
    tf.position += cl.offset;

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
}

void PhysicsEngine::addCustomCollisionResolve(int id, System* ptr) {
    this->customCollisionResolve[id].pushBack(ptr);
}

void PhysicsEngine::setScene(Scene* scene) {
    this->scene = scene;
    this->tfMap = &(this->scene->getComponents<Transform>());
    this->clMap = &(this->scene->getComponents<Collider>());
    this->efMap = &(this->scene->getComponents<EntityFlags>());
}

void PhysicsEngine::setSystems(DynamicArray<System*>* sys){
    this->systems = sys;
}

void PhysicsEngine::setSharedResources(SharedResources* sh) {
    this->sharedResources = sh;
}

CollisionEventMap PhysicsEngine::getAllCollisions(CollisionMap possibleCollisions) {
    if (!possibleCollisions.size()) return{};

    for (Entity entity : this->collisionEntitys) {
        this->entityMinTimesCollisionEventX[entity.getId()] = CollisionEvent();
        this->entityMinTimesCollisionEventY[entity.getId()] = CollisionEvent();
    }
    
    std::map<float, DynamicArray<CollisionEvent>> colEventsMapX;
    std::map<float, DynamicArray<CollisionEvent>> colEventsMapY;

    
    auto collisionHelper = [&](DynamicArray<Entity>& entitys) -> void {
        for (Entity entity: entitys) {
            if (possibleCollisions.find(entity.getId()) == possibleCollisions.end()) continue;
            CollisionEvent& colEventX = this->entityMinTimesCollisionEventX[entity.getId()];
            CollisionEvent& colEventY = this->entityMinTimesCollisionEventY[entity.getId()];
            if (colEventX.time <= 1) {
                for (int i = 0; i < colEventsMapX[colEventX.time].size(); i++) {
                    const CollisionEvent& colEvent = colEventsMapX[colEventX.time].at(i);
					if (colEvent.entity.getId() != entity.getId()) continue;
                    colEventsMapX[colEventX.time].erase(i);
					break;
                }
            }
			if (colEventY.time <= 1) {
				for (int i = 0; i < colEventsMapY[colEventY.time].size(); i++) {
					const CollisionEvent& colEvent = colEventsMapY[colEventY.time].at(i);
					if (colEvent.entity.getId() != entity.getId()) continue;
					colEventsMapY[colEventY.time].erase(i);
					break;
				}
			}

            // Check for collisions
            checkForCollision(entity, possibleCollisions);
        }
        for (Entity entity : entitys) {
			CollisionEvent& colEventX = this->entityMinTimesCollisionEventX[entity.getId()];
			CollisionEvent& colEventY = this->entityMinTimesCollisionEventY[entity.getId()];
			if (colEventX.time <= 1) colEventsMapX[colEventX.time].pushBack(colEventX);
			if (colEventY.time <= 1) colEventsMapY[colEventY.time].pushBack(colEventY);
        }
    };
    
    
    collisionHelper(this->dynamicCollisionEntitys);

    CollisionEventMap collisionEvents;
    for (int i = 0; (colEventsMapX.size() || colEventsMapY.size()); i++) {
        std::unordered_set<int> visited;
        DynamicArray<Entity> newCollisions;
        auto determineNewCollisions = [&](std::map<float, DynamicArray<CollisionEvent>>& colEventsMap, std::unordered_map<int, CollisionEvent>& entityMinTimesCollisionEvent) -> void {
            if (!colEventsMap.size()) return;

            for (const CollisionEvent& colEvent : colEventsMap.begin()->second) {
                int oldId = colEvent.entity.getId();
				entityMinTimesCollisionEvent[oldId] = CollisionEvent();

                if (!this->efMap->at(oldId).getFlag(Dynamic)) continue;

                preventIntersection(colEvent);
                collisionEvents[oldId].pushBack(colEvent);
                for (Entity entity : possibleCollisions[oldId]) {
                    int id = entity.getId();
                    if (!this->efMap->at(id).getFlag(Dynamic)) continue;
                    if (visited.find(id) != visited.end()) continue;
                    visited.insert(id);
                    newCollisions.pushBack(entity);
                }
            }

            colEventsMap.erase(colEventsMap.begin()->first);
        };
		determineNewCollisions(colEventsMapX, this->entityMinTimesCollisionEventX);
		determineNewCollisions(colEventsMapY, this->entityMinTimesCollisionEventY);
        
        collisionHelper(newCollisions);
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

    this->entityMinTimesCollisionEventX.clear();
    this->entityMinTimesCollisionEventY.clear();

	for (Entity entity : this->collisionEntitys) {
		this->entityMinTimesCollisionEventX[entity.getId()];
		this->entityMinTimesCollisionEventY[entity.getId()];
	}
}

void PhysicsEngine::setDynamicCollisionEntitys(DynamicArray<Entity>& entitys) {
    this->dynamicCollisionEntitys = std::move(entitys);
    for (Entity entity : this->dynamicCollisionEntitys) {
		const Collider& cl = this->clMap->at(entity.getId());
		this->minArea += cl.width * cl.height;
    }
	this->minArea /= this->dynamicCollisionEntitys.size();
	this->minArea *= 5; // Magic number to optimize zone size
}

void PhysicsEngine::applyVelocity(int id){
    Transform& tf = this->tfMap->at(id);
    EntityFlags& ef = this->efMap->at(id);
    Vector3D move = tf.velocity;
    if (!ef.getFlag(MovedX)) tf.position.x += move.x;
    if (!ef.getFlag(MovedY)) tf.position.y += move.y;
    ef.setFlag(MovedX, true);
    ef.setFlag(MovedY, true);
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
