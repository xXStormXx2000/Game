#include "Controler.h"

void Controler::onCollision(const CollisionEvent&) {
}

void Controler::start(Entity entity) {
	setCameraWidth(getWindowWidth());
	setCameraHeight(getWindowHeight());
	setCameraOffset({ -getCameraWidth() / 2 + 8, -getCameraHeight() / 2 + 8, 0 });
	setCameraFollowEntity(entity.getId());
	addEntityTag("Player", entity);
	addSound("jump", "Assets/Audio/jump.wav");
}

void Controler::update(Entity entity) {
	Transform& tf = getComponent<Transform>(entity.getId());
	tf.velocity.x = (this->keyDown('d') - this->keyDown('a')) * 3;
	tf.velocity.y = (this->keyDown('s') - this->keyDown('w')) * 3;
	if (this->keyPressed('w')) {
		playSound("jump");
	}
	if (this->keyDown('w')) {
		drawText("Hello World!\nThat's cool, man.", { 100, 100, 1 });
	}
}
