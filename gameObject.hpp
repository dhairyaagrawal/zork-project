/*
 * gameObject.hpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Dhairya
 */

#ifndef GAMEOBJECT_HPP_
#define GAMEOBJECT_HPP_

#include "Room.hpp"
#include "Container.hpp"
#include "Item.hpp"
#include "Creature.hpp"

class gameObject {
public:
	gameObject();
	~gameObject();

	Container inventory;
	std::list<Item*> freeItems;
	std::list<Creature*> freeCreatures;
	std::list<Container*> freeContainers;
	std::list<Room*> rooms;
};

#endif /* GAMEOBJECT_HPP_ */
