/*
 * Container.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Dhairya
 */

#include "Container.hpp"

Container::Container() {open = false; derivedType = "Container";}
Container::~Container() {
	for(std::list<Trigger*>::iterator it = triggers.begin(); it != triggers.end(); ++it) {
		delete (*it);
	}
	for(std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
		delete (*it);
	}
}

void Container::printItems() {
	if(items.begin() == items.end()) {std::cout << name << " is empty\n"; return;}
	std::cout << name << " contains " << items.front()->name;

	std::list<Item*>::iterator it = items.begin();
	for(++it; it != items.end(); ++it) {
		std::cout << ", " << (*it)->name;
	}
	std::cout << std::endl;
}
