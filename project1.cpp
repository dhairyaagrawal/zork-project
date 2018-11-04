//============================================================================
// Name        : project.cpp
// Author      : Dhairya
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

int main(void) {
	rapidxml::file<> file("sample.xml");
	rapidxml::xml_document<> doc;
	doc.parse<0>(file.data());
	rapidxml::print(std::cout, doc, 0);
	return EXIT_SUCCESS;
}
