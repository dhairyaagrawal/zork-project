/*
 * Container.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include "Base.hpp"
#include "Item.hpp"
#include "Trigger.hpp"
#include <string>
#include <set>
#include <list>

class Container : public Base {
public:
	Container();
	virtual ~Container();

	std::string name;
	std::string status;
	std::string description;
	std::set<std::string> accept;  //if (accept.find(myinput) != accept.end())
	std::list<Item*> items;
	std::list<Trigger*> triggers;
};

#endif /* CONTAINER_HPP_ */
