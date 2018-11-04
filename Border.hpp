/*
 * Border.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Dhairya
 */

#ifndef BORDER_HPP_
#define BORDER_HPP_

#include <string>

class Border {
public:
	Border();
	virtual ~Border();

	std::string name;
	std::string direction;
	//Room *ref;
};

#endif /* BORDER_HPP_ */
