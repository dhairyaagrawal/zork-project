/*
 * Room.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#include "Room.hpp"

Room::Room() {type = false; derivedType = "Room";}
Room::~Room() {
	for(std::list<Trigger*>::iterator it = triggers.begin(); it != triggers.end(); ++it) {
		delete (*it);
	}
	for(std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
		delete (*it);
	}
	for(std::list<Creature*>::iterator it = creatures.begin(); it != creatures.end(); ++it) {
		delete (*it);
	}
	for(std::list<Container*>::iterator it = containers.begin(); it != containers.end(); ++it) {
		delete (*it);
	}
}
