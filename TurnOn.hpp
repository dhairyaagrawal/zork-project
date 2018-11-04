/*
 * TurnOn.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#ifndef TURNON_HPP_
#define TURNON_HPP_

#include <string>
#include <list>

class TurnOn {
public:
	TurnOn();
	virtual ~TurnOn();

	bool exists; //0->doesn't exist, 1->exists
	std::list<std::string> prints;
	std::list<std::string> actions;
};



#endif /* TURNON_HPP_ */
