#include "Softbody.h"

void Softbody::onCollision(const CollisionEvent& colEvent) {
	Transform& tf = this->getComponent<Transform>(colEvent.entity.getId());
	
	if (colEvent.collisionDirection.x) {
		tf.velocity.x = 0;
	}
	if (colEvent.collisionDirection.y) {
		tf.velocity.y = 0;

	}
}


void Softbody::update(Entity entity) {
	Transform& tf = getComponent<Transform>(entity.getId());
	//if (this->softbodys[0][0].getId() != entity.getId()) return;
	somData& sD = getComponent<somData>(entity.getId());
	debugMessage(sD.data);

	if (tf.velocity.y < 20) tf.velocity.y += 1;
	if(this->keyDown('d') || this->keyDown('a')) tf.velocity.x = (this->keyDown('d') - this->keyDown('a')) * 7;
	if(this->keyDown('s') || this->keyDown('w')) tf.velocity.y = (this->keyDown('s') - this->keyDown('w')) * 7;
	//if (this->keyPressed('w')) tf.velocity.y = -20;
}
