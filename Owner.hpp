/*
 * Owner.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: Dhairya
 */

#ifndef OWNER_HPP_
#define OWNER_HPP_

#include <string>

class Owner {
public:
	Owner();
	virtual ~Owner();

	bool has; //0->no, 1->yes
	std::string objName;
	std::string owner;
};

#endif /* OWNER_HPP_ */
