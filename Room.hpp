/*
 * Room.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#ifndef ROOM_HPP_
#define ROOM_HPP_

#include <list>
#include "Base.hpp"
#include "Border.hpp"
#include "Container.hpp"
#include "Item.hpp"
#include "Creature.hpp"
#include "Trigger.hpp"

class Room : public Base {
public:
	Room();
	virtual ~Room();

	bool type; //0->regular, 1->exit
	std::list<Border> borders;
	std::list<Container*> containers;
	std::list<Item*> items;
	std::list<Creature*> creatures;
};



#endif /* ROOM_HPP_ */
