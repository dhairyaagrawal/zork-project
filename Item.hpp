/*
 * Item.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "Base.hpp"
#include "TurnOn.hpp"
#include "Trigger.hpp"
#include <list>

class Item : public Base {
public:
	Item();
	virtual ~Item();

	std::string writing;
	TurnOn turnon;
};

#endif /* ITEM_HPP_ */
