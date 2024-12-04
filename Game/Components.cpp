#include "Components.h"

bool EntityFlags::getActive() {
	return this->flags & 0b0000'0001;
}

void EntityFlags::setActive(bool input) {
	this->flags &= ~0b0000'0001;
	this->flags |= char(input);
}

bool EntityFlags::getVisible() {
	return this->flags & 0b0000'0010;
}

void EntityFlags::setVisible(bool input) {
	this->flags &= ~0b0000'0010;
	this->flags |= char(input) << 1;
}

bool EntityFlags::getMuted() {
	return this->flags & 0b0000'0100;
}

void EntityFlags::setMuted(bool input) {
	this->flags &= ~0b0000'0100;
	this->flags |= char(input) << 2;
}

bool EntityFlags::getSolid() {
	return this->flags & 0b0000'1000;
}

void EntityFlags::setSolid(bool input) {
	this->flags &= ~0b0000'1000;
	this->flags |= char(input) << 3;
}

bool EntityFlags::getMovedX() {
	return this->flags & 0b0001'0000;
}

void EntityFlags::setMovedX(bool input) {
	this->flags &= ~0b0001'0000;
	this->flags |= char(input) << 4;
}

bool EntityFlags::getMovedY() {
	return this->flags & 0b0010'0000;
}

void EntityFlags::setMovedY(bool input) {
	this->flags &= ~0b0010'0000;
	this->flags |= char(input) << 5;
}
