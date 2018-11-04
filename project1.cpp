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

	//PRINT XML FILE
	//std::cout << doc; 
	//rapidxml::print(std::cout, doc, 0);
	
	std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
	rapidxml::xml_node<> *node = doc.first_node("map");
	std::cout << "Node map has value " << node->value() << "\n";
	for (rapidxml::xml_node<> *element = node->first_node(); element; element = element->next_sibling()) {
		std::cout << "Node map has element " << element->name() << " ";
		std::cout << "with value " << element->value() << "\n";
		}
	return EXIT_SUCCESS;
}
