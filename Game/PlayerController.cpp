#include "PlayerController.h"

void  PlayerController::onCollision(const CollisionEvent& colEvent) {
	Transform& tf = this->getComponent<Transform>(colEvent.entity.getId());
	if (colEvent.collisionDirection.x) {
		tf.velocity.x = 0;
	}
	if (colEvent.collisionDirection.y) {
		tf.velocity.y = 0;
	}
}

void PlayerController::update(Entity entity) {
	Transform& tf = getComponent<Transform>(entity.getId());

	tf.velocity.x = (this->keyDown('d') - this->keyDown('a'));
	tf.velocity.y = (this->keyDown('s') - this->keyDown('w'));
	if(tf.velocity.abs()) tf.velocity.normalize();
	tf.velocity *= speed;


}
