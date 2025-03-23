#include "SoftbodySpawner.h"

SoftbodySpawner::SoftbodySpawner() : softbodys(16, DynamicArray<Entity>(16)) {

}

void SoftbodySpawner::start(Entity) {

	setCameraWidth(getCameraWidth()*2);
	setCameraHeight(getCameraHeight()*2);
	setCameraOffset({-getCameraWidth()/2, -getCameraHeight()/2, 0});

	for (int i = 0; i < softbodys.size(); i++) {
		for (int j = 0; j < softbodys[0].size(); j++) {
			Entity entity = createEntity();
			
			if (i == softbodys.size() / 2 && j == softbodys[0].size() / 2)
				setCameraFollowEntity(entity);

			EntityFlags* ef = new EntityFlags;
			ef->flags = 27;

			addEntity(entity, ef);
			
			addEntityToSystem(entity, 0);

			Transform* tf = new Transform;
			addComponentToEntity(entity, tf);
			tf->scale = { 1, 1, 0 };
			tf->position.x = i * 30 + 50;
			tf->position.y = j * 30 + 50;

			Collider* cl = new Collider;
			addComponentToEntity(entity, cl);
			cl->height = 5;
			cl->width = 5;


			somData* sD = new somData;
			addComponentToEntity(entity, sD);
			sD->data = rand() % 10000;

			Sprite* sp = new Sprite;
			addComponentToEntity(entity, sp);
			sp->height = 5;
			sp->width = 5;
			sp->texturePortion.x = 0;
			sp->texturePortion.y = 0;
			sp->texturePortion.w = 64;
			sp->texturePortion.h = 64;
			sp->spriteIndex = 0;

			addEntityToRenderer(entity);

			addEntityToPhysicsEngine(entity);

			this->softbodys[i][j] = entity;
			this->posMap[entity.getId()] = { i, j };

		}
	}
}


void SoftbodySpawner::preUpdate(Entity) {
	float dist = 30;
	for (auto [entity, xy] : this->posMap) {
		Transform& tf = getComponent<Transform>(entity);
		int x = xy.first;
		int y = xy.second;
		for (int i = -1; i < 2; i++) {
			if (i + y < 0 || i + y >= this->softbodys[0].size()) continue;
			for (int j = -1; j < 2; j++) {
				if (!i && !j) continue;
				if (j + x < 0 || j + x >= this->softbodys.size()) continue;
				Entity other = this->softbodys[j + x][i + y];
				Transform otherTf = getComponent<Transform>(other.getId());
				if (j) {
					float vel = (otherTf.velocity.x - tf.velocity.x) * j;
					vel = spring(vel, (otherTf.position.x - tf.position.x) * j, dist, 0.3, 0.1);
					tf.velocity.x += vel * j;
				}
				if (i) {
					float vel = (otherTf.velocity.y - tf.velocity.y) * i;
					vel = spring(vel, (otherTf.position.y - tf.position.y) * i, dist, 0.3, 0.1);
					tf.velocity.y += vel * i;
				}
			}
		}
	}
}