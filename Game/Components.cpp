#include "Components.h"

bool EntityFlags::getFlag(int flag) const {
	return this->flags & (1 << flag);
}

void EntityFlags::setFlag(int flag, bool input) {
	this->flags &= ~(1 << flag);
	this->flags |= char(input) << flag;
}

bool EntityFlags::checkFlags(int flagsMask) const {
	return flagsMask == (this->flags&flagsMask);
}

Component* EntityFlags::readFile(std::ifstream& file, std::string& str) {
	return nullptr;
}

void EntityFlags::writeFile(std::ofstream& str) {
}


Component* Transform::readFile(std::ifstream& file, std::string& str)
{
	if (str == "Transform") {
		Transform* tf = new Transform;
		file >> tf->position.x >> tf->position.y >> tf->position.z;
		file >> tf->velocity.x >> tf->velocity.y >> tf->velocity.z;
		//file >> tf.rotation.x >> tf.rotation.y >> tf.rotation.z;
		file >> tf->scale.x >> tf->scale.y >> tf->scale.z;
		return dynamic_cast<Component*>(tf);
	}
	return nullptr;
}

void Transform::writeFile(std::ofstream& file) {
	file << "Transform\n";
	file << this->position.x << ' ' << this->position.y << ' ' << this->position.z << '\n';
	file << this->velocity.x << ' ' << this->velocity.y << ' ' << this->velocity.z << '\n';
	//file << this->rotation.x << ' ' << this->rotation.y << ' ' << this->rotation.z << '\n';
	file << this->scale.x << ' ' << this->scale.y << ' ' << this->scale.z << '\n';
}

Component* Collider::readFile(std::ifstream& file, std::string& str) {
	if (str == "Collider") {
		Collider* cl = new Collider;
		file >> cl->offset.x >> cl->offset.y >> cl->offset.z;
		file >> cl->width >> cl->height;
		return dynamic_cast<Component*>(cl);
	}
	return nullptr;
}

void Collider::writeFile(std::ofstream& file) {
	file << "Collider\n";
	file << this->offset.x << ' ' << this->offset.y << ' ' << this->offset.z << '\n';
	file << this->width << ' ' << this->height << '\n';
}

Component* Rigidbody::readFile(std::ifstream& file, std::string& str) {

	if (str == "Rigidbody") {
		Rigidbody* rb = new Rigidbody;
		file >> rb->mass >> rb->density >> rb->friction >> rb->restitution;
		file >> rb->centerOfMass.x >> rb->centerOfMass.y >> rb->centerOfMass.z;
		file >> rb->acceleration.x >> rb->acceleration.y >> rb->acceleration.z;
		//file >> rb->angularVelocity.x >> rb->angularVelocity.y >> rb->angularVelocity.z;
		return dynamic_cast<Component*>(rb);
	}
	return nullptr;
}

void Rigidbody::writeFile(std::ofstream& file) {
	file << "Rigidbody\n";
	file << this->mass << ' ' << this->density << ' ' << this->friction << ' ' << this->restitution << '\n';
	file << this->centerOfMass.x << ' ' << this->centerOfMass.y << ' ' << this->centerOfMass.z << '\n';
	file << this->acceleration.x << ' ' << this->acceleration.y << ' ' << this->acceleration.z << '\n';
	//file << this->angularVelocity.x << ' ' << this->angularVelocity.y << ' ' << this->angularVelocity.z << '\n';
}

Component* Sprite::readFile(std::ifstream& file, std::string& str) {
	if (str == "Sprite") {
		Sprite* sr = new Sprite;
		file >> sr->offset.x >> sr->offset.y >> sr->offset.z;
		file >> sr->width >> sr->height;
		file >> sr->texturePortion.x >> sr->texturePortion.y >> sr->texturePortion.w >> sr->texturePortion.h;
		file >> sr->spriteIndex;
		return dynamic_cast<Component*>(sr);
	}
	return nullptr;
}

void Sprite::writeFile(std::ofstream& file) {
	file << "Sprite\n";
	file << this->offset.x << ' ' << this->offset.y << ' ' << this->offset.z << '\n';
	file << this->width << ' ' << this->height << '\n';
	file << this->texturePortion.x << ' ' << this->texturePortion.y << ' ' << this->texturePortion.w << ' ' << this->texturePortion.h << '\n';
	file << this->spriteIndex << '\n';
}

Component* TileSet::readFile(std::ifstream& file, std::string& str) {
	if (str == "TileSet") {
		TileSet* ts = new TileSet;
		file >> ts->offset.x >> ts->offset.y >> ts->offset.z;
		file >> ts->tileWidth >> ts->tileHeight;
		file >> ts->textureTileWidth >> ts->textureTileHeight;
		file >> ts->spriteIndex;
		int size;
		file >> size;
		for (int i = 0; i < size; i++) {
			file >> ts->tiles[i].x >> ts->tiles[i].y >> ts->tiles[i].z;
		}
		return dynamic_cast<Component*>(ts);
	}
	return nullptr;
}

void TileSet::writeFile(std::ofstream& file) {
	file << "TileSet\n";
	file << this->offset.x << ' ' << this->offset.y << ' ' << this->offset.z << '\n';
	file << this->tileWidth << ' ' << this->tileHeight << '\n';
	file << this->textureTileWidth << ' ' << this->textureTileHeight << '\n';
	file << this->spriteIndex << '\n';
	file << this->tiles.size() << '\n';
	for (int i = 0; i < this->tiles.size(); i++) {
		file << this->tiles[i].x << ' ' << this->tiles[i].y << ' ' << this->tiles[i].z << '\n';
	}
}

