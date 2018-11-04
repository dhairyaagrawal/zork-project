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
public:
	Status();
	virtual ~Status();

	std::string status;
	std::string objName;
};

#endif /* STATUS_HPP_ */
