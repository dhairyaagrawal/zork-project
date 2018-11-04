/*
 * Attack.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Dhairya
 */

#ifndef ATTACK_HPP_
#define ATTACK_HPP_

#include <string>
#include <list>
#include "Status.hpp"

class Attack {
public:
	Attack();
	virtual ~Attack();

	Status statusCond; //more than one??
	std::list<std::string> prints;
	std::list<std::string> actions;
};



#endif /* ATTACK_HPP_ */
