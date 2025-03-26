#include "SoftbodyNode.h"


void SoftbodyNode::onCollision(const CollisionEvent& colEvent) {
	Transform& tf = this->getComponent<Transform>(colEvent.entity.getId());
	if (colEvent.collisionDirection.x) {
		tf.velocity.x = 2*colEvent.collisionDirection.x;
	}
	if (colEvent.collisionDirection.y) {
		tf.velocity.y = 2*colEvent.collisionDirection.y;
	}
}

void SoftbodyNode::update(Entity entity) {
	Transform& tf = getComponent<Transform>(entity.getId());
	somData& sD = getComponent<somData>(entity.getId());
	//debugMessage(sD.data);

	//if (tf.velocity.y < 20) tf.velocity.y += 1;
	//if(this->keyDown('d') || this->keyDown('a')) tf.velocity.x = (this->keyDown('d') - this->keyDown('a')) * 7;
	//if (this->keyPressed('w')) tf.velocity.y = -20;
}
