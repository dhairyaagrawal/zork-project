/*
 * Item.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Dhairya
 */

#include "Item.hpp"

Item::Item() {derivedType = "Item";}
Item::~Item() {
	for(std::list<Trigger*>::iterator it = triggers.begin(); it != triggers.end(); ++it) {
		delete (*it);
	}
}
