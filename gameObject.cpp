/*
 * gameObject.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Dhairya
 */

#include "gameObject.hpp"

gameObject::gameObject() {
	inventory.name = "inventory";
	inventory.open = true;
}

gameObject::~gameObject() {
	for(std::list<Item*>::iterator it = freeItems.begin(); it != freeItems.end(); ++it) {
		delete (*it);
	}
	for(std::list<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
		delete (*it);
	}
	for(std::list<Creature*>::iterator it = freeCreatures.begin(); it != freeCreatures.end(); ++it) {
		delete (*it);
	}
	for(std::list<Container*>::iterator it = freeContainers.begin(); it != freeContainers.end(); ++it) {
		delete (*it);
	}
}


