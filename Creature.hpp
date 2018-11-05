/*
 * Creature.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Dhairya
 */

#ifndef CREATURE_HPP_
#define CREATURE_HPP_

#include "Base.hpp"
#include "Attack.hpp"
#include "Trigger.hpp"
#include <string>
#include <list>

class Creature : public Base {
public:
	Creature();
	virtual ~Creature();

	std::string name;
	std::string status;
	std::string description;
	std::list<std::string> weaknesses;
	Attack attack;
};

#endif /* CREATURE_HPP_ */
