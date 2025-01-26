#include "Softbody.h"

Softbody::Softbody(): softbodys(64, DynamicArray<Entity>(64)){
}

void Softbody::onCollision(const CollisionEvent& colEvent) {
	Transform& tf = this->getComponent<Transform>(colEvent.entity.getId());
	
	if (colEvent.collisionDirection.x) {
		tf.velocity.x = 0.5 * abs(tf.velocity.x) * colEvent.collisionDirection.x;
		if (abs(tf.velocity.x) < 1) tf.velocity.x = 0;
	}
	if (colEvent.collisionDirection.y) {
		tf.velocity.y = 0.5 * abs(tf.velocity.y) * colEvent.collisionDirection.y;
		if (abs(tf.velocity.y) < 1) tf.velocity.y = 0;

	}
}

void Softbody::start(Entity entity) {
	if (i < 64 && j < 64) {
		Transform& tf = getComponent<Transform>(entity.getId());
		this->softbodys[i][j] = entity;
		this->posMap[entity.getId()] = {i, j};
		tf.position.x = i * 10 + 50;
		tf.position.y = j * 10 + 50;
	}
	
	i++;
	if (i == 48) {
		i = 0;
		j++;
	}
}

void Softbody::preUpdate(Entity entity) {
	/*float dist = 50;
	Transform& tf = getComponent<Transform>(entity.getId());
	auto [x, y] = this->posMap[entity.getId()];
	for (int i = -1; i < 2; i++) {
		if (i + y < 0 || i + y >= this->softbodys[0].size()) continue;
		for (int j = -1; j < 2; j++) {
			if (!i && !j) continue;
			if (j + x < 0 || j + x >= this->softbodys.size()) continue;
			Entity other = this->softbodys[j + x][i + y];
			Transform otherTf = getComponent<Transform>(other.getId());
			if (j) {
				float vel = (otherTf.velocity.x - tf.velocity.x)*j;
				vel = spring(vel, (otherTf.position.x - tf.position.x)*j, dist, 2, 0.01);
				tf.velocity.x += vel*j * this->getDeltaTime()*120;
			}
			if (i) {
				float vel = (otherTf.velocity.y - tf.velocity.y)*i;
				vel = spring(vel, (otherTf.position.y - tf.position.y)*i, dist, 2, 0.01);
				tf.velocity.y += vel*i * this->getDeltaTime()*120;
			}
		}
	}*/
}

void Softbody::update(Entity entity) {
	Transform& tf = getComponent<Transform>(entity.getId());
	//if (this->softbodys[0][0].getId() != entity.getId()) return;
	if (tf.velocity.y < 20) tf.velocity.y += 1;
	if(this->keyDown('d') || this->keyDown('a')) tf.velocity.x = (this->keyDown('d') - this->keyDown('a')) * 7;
	if(this->keyDown('s') || this->keyDown('w')) tf.velocity.y = (this->keyDown('s') - this->keyDown('w')) * 7;
	//if (this->keyPressed('w')) tf.velocity.y = -20;
}
