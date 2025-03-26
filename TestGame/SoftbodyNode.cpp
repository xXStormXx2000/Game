#include "SoftbodyNode.h"


void SoftbodyNode::onCollision(const CollisionEvent& colEvent) {
	Transform& tf = this->getComponent<Transform>(colEvent.entity.getId());
	if (colEvent.collisionDirection.x) {
		tf.velocity.x = 0.75 * colEvent.collisionDirection.x;
		if (colEvent.other.getId() == getTagEntitys("Player").begin()->getId()) {
			const Transform& otherTf = this->getComponent<Transform>(colEvent.other.getId());
			if(otherTf.velocity.x != 0) tf.velocity.x = otherTf.velocity.x;
		}
	}
	if (colEvent.collisionDirection.y) {
		tf.velocity.y = 0.75*colEvent.collisionDirection.y;
		if (colEvent.other.getId() == getTagEntitys("Player").begin()->getId()) {
			const Transform& otherTf = this->getComponent<Transform>(colEvent.other.getId());
			if (otherTf.velocity.y != 0) tf.velocity.y = otherTf.velocity.y;
		}
	}
}

void SoftbodyNode::update(Entity entity) {
	Transform& tf = getComponent<Transform>(entity.getId());
	somData& sD = getComponent<somData>(entity.getId());
	if (tf.velocity * tf.velocity == 0) {
		tf.velocity = { 0.75f * float(rand() % 3 - 1), 0.75f * float(rand() % 3 - 1), 0 };
	}
	//debugMessage(sD.data);

	//if (tf.velocity.y < 20) tf.velocity.y += 1;
	//if(this->keyDown('d') || this->keyDown('a')) tf.velocity.x = (this->keyDown('d') - this->keyDown('a')) * 7;
	//if (this->keyPressed('w')) tf.velocity.y = -20;
}
