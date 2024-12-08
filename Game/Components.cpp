#include "Components.h"

bool EntityFlags::getFlag(Flags flag) const {
	return this->flags & (1 << flag);
}

void EntityFlags::setFlag(Flags flag, bool input) {
	this->flags &= ~(1 << flag);
	this->flags |= char(input) << flag;
}

bool EntityFlags::checkFlags(int flagsMask) const {
	return flagsMask == (this->flags&flagsMask);
}

bool EntityFlags::haveComponent(ComponentFlags flag) const {
	return this->components & (1 << flag);
}

bool EntityFlags::checkComponents(int flagsMask) const {
	return flagsMask == (this->components & flagsMask);
}
