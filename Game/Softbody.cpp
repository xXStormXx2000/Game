#include "Softbody.h"

void Softbody::onCollision(const CollisionEvent& colEvent) {
	Transform& tf = this->getComponent<Transform>(colEvent.entity.getId());
	if (colEvent.collisionDirection.x) {
		tf.velocity.x *= 0;
	}
	if (colEvent.collisionDirection.y) {
		tf.velocity.y *= 0;
	}
}

void Softbody::start(Entity emtity) {
	softbodys.pushBack(emtity);
}

void Softbody::preUpdate(Entity entity) {
	float dist = 150;
	Transform& tf = getComponent<Transform>(entity.getId());
	tf.velocity.x = (this->keyDown('d') - this->keyDown('a')) * 200;
	if (this->keyDown('w')) tf.velocity.y = -500;
	for (Entity other : softbodys) {
		if (other.getId() == entity.getId()) continue;
		Transform otherTf = getComponent<Transform>(other.getId());
		Vector3D dir = (otherTf.position - tf.position).normalize();
		Vector3D vel = otherTf.velocity - tf.velocity;
		float v = dir * vel;
		v = spring(v, tf.position.distance(otherTf.position), dist, 1, 0.01);
		tf.velocity += dir * v;
	}
}

void Softbody::update(Entity entity) {
	Transform& tf = getComponent<Transform>(entity.getId());
	if (tf.velocity.y < 500) tf.velocity.y += 30;


	if (entity.getId() != softbodys[0].getId()) return;
	Transform otherTf = getComponent<Transform>(softbodys[3].getId());
	Collider otherCl = getComponent<Collider>(softbodys[3].getId());
	Sprite& spr = getComponent<Sprite>(entity.getId());
	spr.width = otherTf.position.x - tf.position.x + otherCl.width;
	spr.height = otherTf.position.y - tf.position.y + otherCl.height;

}
