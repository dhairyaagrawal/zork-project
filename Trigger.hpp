/*
 * Trigger.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#ifndef TRIGGER_HPP_
#define TRIGGER_HPP_

#include "Base.hpp"
#include "Owner.hpp"
#include "Status.hpp"
#include <string>
#include <list>

class Trigger : public Base {
public:
	Trigger();
	~Trigger();

	Owner ownerCond;
	Status statusCond;
	std::string command;
	bool type; //0->Once, 1->Permanent
	bool used; //0->Not used, 1->Used (only if type is 0)
	std::list<std::string> prints;
	std::list<std::string> actions;
};



#endif /* TRIGGER_HPP_ */
