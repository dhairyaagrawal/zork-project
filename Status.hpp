/*
 * Status.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Dhairya
 */

#ifndef STATUS_HPP_
#define STATUS_HPP_

#include <string>

class Status {
	Status();
	virtual ~Status();

	bool has; //0->no, 1->yes
	std::string objName;
	std::string owner;
};

#endif /* STATUS_HPP_ */
