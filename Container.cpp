/*
 * Container.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Dhairya
 */

#include "Container.hpp"

Container::Container() {open = false;}
Container::~Container() {
	for(std::list<Trigger*>::iterator it = triggers.begin(); it != triggers.end(); ++it) {
		delete (*it);
	}
	for(std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
		delete (*it);
	}
}
