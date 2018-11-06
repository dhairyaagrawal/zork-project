/*
 * Base.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#ifndef BASE_HPP_
#define BASE_HPP_

#include <string>
#include "Trigger.hpp"
#include <list>

class Base {
public:
	Base();
	virtual ~Base();

	std::string name;
	std::string status;
	std::string description;
	std::string derivedType;
	std::list<Trigger*> triggers;
};

#endif /* BASE_HPP_ */
