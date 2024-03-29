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
#include <list>
#include "iostream"

class Container : public Base {
public:
	Container();
	virtual ~Container();
	void printItems();

	std::list<std::string> accept;  //if (accept.find(myinput) != accept.end()) for sets
	std::list<Item*> items;
	bool open; //0->closed, 1->open
};

#endif /* CONTAINER_HPP_ */
