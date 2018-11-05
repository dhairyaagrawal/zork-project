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
#include <list>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "gameObject.hpp"

gameObject* createGame (rapidxml::xml_node<>*);
void rungame(gameObject*);
bool execute_command(gameObject*, std::string);

Room* searchRoom(std::list<Room*>*, std::string);
Item* searchItem(std::list<Item*>*, std::string);
Container* searchContainer(std::list<Container*>*, std::string);
Creature* searchCreature(std::list<Creature*>*, std::string);

int main(int argc, char* argv[]) {
	//READ XML FILE INTO A DOM TREE ROOT
	rapidxml::file<> file(argv[1]);
	rapidxml::xml_document<> doc;
	doc.parse<0>(file.data());
	rapidxml::xml_node<> *root = doc.first_node("map");

	//PRINT COMPLETE XML FILE AUTOMATICALLY
	//std::cout << doc; 
	//rapidxml::print(std::cout, doc, 0);
	
	//how to parse DOM
	gameObject* game = createGame(root);

	rungame(game);
	return EXIT_SUCCESS;
}

gameObject* createGame(rapidxml::xml_node<> *map) {
	gameObject* game = new gameObject();

	//Create items
	for (rapidxml::xml_node<> *element = map->first_node("item"); element; element = element->next_sibling("item")) {
		Item *newItem = new Item();
		newItem->name = element->first_node("name")->value();
		newItem->status = element->first_node("status") ? element->first_node("status")->value() : "";
		newItem->writing = element->first_node("writing") ? element->first_node("writing")->value() : "";
		newItem->description = element->first_node("description") ? element->first_node("description")->value() : "";

		if(element->first_node("turnon")) {
			newItem->turnon.exists = true;
			for(rapidxml::xml_node<>* tmp = element->first_node("turnon")->first_node("print"); tmp; tmp = tmp->next_sibling("print")) {
				newItem->turnon.prints.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* tmp = element->first_node("turnon")->first_node("action"); tmp; tmp = tmp->next_sibling("action")) {
				newItem->turnon.actions.push_back(tmp->value());
			}
		}

		for(rapidxml::xml_node<>* trigger = element->first_node("trigger"); trigger; trigger = trigger->next_sibling("trigger")) {
			Trigger* newTrigger = new Trigger();
			newTrigger->command = trigger->first_node("command") ? trigger->first_node("command")->value() : "";
			newTrigger->type = (trigger->first_node("type")) ? true : false;
			for(rapidxml::xml_node<>* tmp = trigger->first_node("print"); tmp; tmp = tmp->next_sibling("print")) {
				newTrigger->prints.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* tmp = trigger->first_node("action"); tmp; tmp = tmp->next_sibling("action")) {
				newTrigger->actions.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* cond = trigger->first_node("condition"); cond; cond = cond->next_sibling("condition")) {
				if(cond->first_node("has")) {
					newTrigger->ownerCond.has = (std::string(cond->first_node("has")->value()) == "yes") ? true : false;
					newTrigger->ownerCond.objName = cond->first_node("object")->value();
					newTrigger->ownerCond.owner = cond->first_node("owner")->value();
				} else {
					newTrigger->statusCond.objName = cond->first_node("object")->value();
					newTrigger->statusCond.status = cond->first_node("status")->value();
				}
			}
			newItem->triggers.push_back(newTrigger);
		}
		game->freeItems.push_back(newItem);
	}

	//Create Creatures
	for (rapidxml::xml_node<> *element = map->first_node("creature"); element; element = element->next_sibling("creature")) {
		Creature *newCreature = new Creature();
		newCreature->name = element->first_node("name")->value();
		newCreature->status = element->first_node("status") ? element->first_node("status")->value() : "";
		newCreature->description = element->first_node("description") ? element->first_node("description")->value() : "";

		for(rapidxml::xml_node<>* tmp = element->first_node("vulnerability"); tmp; tmp = tmp->next_sibling("vulnerability")) {
			newCreature->weaknesses.push_back(tmp->value());
		}
		if(element->first_node("attack")) {
			for(rapidxml::xml_node<>* tmp = element->first_node("attack")->first_node("print"); tmp; tmp = tmp->next_sibling("print")) {
				newCreature->attack.prints.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* tmp = element->first_node("attack")->first_node("action"); tmp; tmp = tmp->next_sibling("action")) {
				newCreature->attack.actions.push_back(tmp->value());
			}
			if(element->first_node("attack")->first_node("condition")) {
				newCreature->attack.statusCond.objName = element->first_node("attack")->first_node("condition")->first_node("object")->value();
				newCreature->attack.statusCond.status = element->first_node("attack")->first_node("condition")->first_node("status")->value();
			}
		}

		for(rapidxml::xml_node<>* trigger = element->first_node("trigger"); trigger; trigger = trigger->next_sibling("trigger")) {
			Trigger* newTrigger = new Trigger();
			newTrigger->command = trigger->first_node("command") ? trigger->first_node("command")->value() : "";
			newTrigger->type = (trigger->first_node("type")) ? true : false;
			for(rapidxml::xml_node<>* tmp = trigger->first_node("print"); tmp; tmp = tmp->next_sibling("print")) {
				newTrigger->prints.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* tmp = trigger->first_node("action"); tmp; tmp = tmp->next_sibling("action")) {
				newTrigger->actions.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* cond = trigger->first_node("condition"); cond; cond = cond->next_sibling("condition")) {
				if(cond->first_node("has")) {
					newTrigger->ownerCond.has = (std::string(cond->first_node("has")->value()) == "yes") ? true : false;
					newTrigger->ownerCond.objName = cond->first_node("object")->value();
					newTrigger->ownerCond.owner = cond->first_node("owner")->value();
				} else {
					newTrigger->statusCond.objName = cond->first_node("object")->value();
					newTrigger->statusCond.status = cond->first_node("status")->value();
				}
			}
			newCreature->triggers.push_back(newTrigger);
		}
		game->freeCreatures.push_back(newCreature);
	}

	//Create Containers
	for (rapidxml::xml_node<> *element = map->first_node("container"); element; element = element->next_sibling("container")) {
		Container *newContainer = new Container();
		newContainer->name = element->first_node("name")->value();
		newContainer->status = element->first_node("status") ? element->first_node("status")->value() : "";
		newContainer->description = element->first_node("description") ? element->first_node("description")->value() : "";

		for(rapidxml::xml_node<>* tmp = element->first_node("accept"); tmp; tmp = tmp->next_sibling("accept")) {
			newContainer->accept.push_back(tmp->value());
		}

		for(rapidxml::xml_node<>* trigger = element->first_node("trigger"); trigger; trigger = trigger->next_sibling("trigger")) {
			Trigger* newTrigger = new Trigger();
			newTrigger->command = trigger->first_node("command") ? trigger->first_node("command")->value() : "";
			newTrigger->type = (trigger->first_node("type")) ? true : false;
			for(rapidxml::xml_node<>* tmp = trigger->first_node("print"); tmp; tmp = tmp->next_sibling("print")) {
				newTrigger->prints.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* tmp = trigger->first_node("action"); tmp; tmp = tmp->next_sibling("action")) {
				newTrigger->actions.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* cond = trigger->first_node("condition"); cond; cond = cond->next_sibling("condition")) {
				if(cond->first_node("has")) {
					newTrigger->ownerCond.has = (std::string(cond->first_node("has")->value()) == "yes") ? true : false;
					newTrigger->ownerCond.objName = cond->first_node("object")->value();
					newTrigger->ownerCond.owner = cond->first_node("owner")->value();
				} else {
					newTrigger->statusCond.objName = cond->first_node("object")->value();
					newTrigger->statusCond.status = cond->first_node("status")->value();
				}
			}
			newContainer->triggers.push_back(newTrigger);
		}

		for(rapidxml::xml_node<>* item = element->first_node("item"); item; item = item->next_sibling("item")) {
			for(std::list<Item*>::iterator it=game->freeItems.begin(); it != game->freeItems.end(); ++it) {
				if((*it)->name == std::string(item->value())) {
					newContainer->items.push_back(*it);
					game->freeItems.remove(*it);
					break;
				}
			}
		}
		game->freeContainers.push_back(newContainer);
	}

	//Create Rooms
	for (rapidxml::xml_node<> *element = map->first_node("room"); element; element = element->next_sibling("room")) {
		Room *newRoom = new Room();
		newRoom->name = element->first_node("name")->value();
		newRoom->status = element->first_node("status") ? element->first_node("status")->value() : "";
		newRoom->type = element->first_node("type") ? true : false;
		newRoom->description = element->first_node("description") ? element->first_node("description")->value() : "";

		for(rapidxml::xml_node<>* trigger = element->first_node("trigger"); trigger; trigger = trigger->next_sibling("trigger")) {
			Trigger* newTrigger = new Trigger();
			newTrigger->command = trigger->first_node("command") ? trigger->first_node("command")->value() : "";
			newTrigger->type = (trigger->first_node("type")) ? true : false;
			for(rapidxml::xml_node<>* tmp = trigger->first_node("print"); tmp; tmp = tmp->next_sibling("print")) {
				newTrigger->prints.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* tmp = trigger->first_node("action"); tmp; tmp = tmp->next_sibling("action")) {
				newTrigger->actions.push_back(tmp->value());
			}
			for(rapidxml::xml_node<>* cond = trigger->first_node("condition"); cond; cond = cond->next_sibling("condition")) {
				if(cond->first_node("has")) {
					newTrigger->ownerCond.has = (std::string(cond->first_node("has")->value()) == "yes") ? true : false;
					newTrigger->ownerCond.objName = cond->first_node("object")->value();
					newTrigger->ownerCond.owner = cond->first_node("owner")->value();
				} else {
					newTrigger->statusCond.objName = cond->first_node("object")->value();
					newTrigger->statusCond.status = cond->first_node("status")->value();
				}
			}
			newRoom->triggers.push_back(newTrigger);
		}

		for(rapidxml::xml_node<>* border = element->first_node("border"); border; border = border->next_sibling("border")) {
			Border newBorder;
			newBorder.name = border->first_node("name")->value();
			newBorder.direction = border->first_node("direction")->value();
			newRoom->borders.push_back(newBorder);
		}

		for(rapidxml::xml_node<>* item = element->first_node("item"); item; item = item->next_sibling("item")) {
			for(std::list<Item*>::iterator it=game->freeItems.begin(); it != game->freeItems.end(); ++it) {
				if((*it)->name == std::string(item->value())) {
					newRoom->items.push_back(*it);
					game->freeItems.remove(*it);
					break;
				}
			}
		}

		for(rapidxml::xml_node<>* container = element->first_node("container"); container; container = container->next_sibling("container")) {
			for(std::list<Container*>::iterator it=game->freeContainers.begin(); it != game->freeContainers.end(); ++it) {
				if((*it)->name == std::string(container->value())) {
					newRoom->containers.push_back(*it);
					game->freeContainers.remove(*it);
					break;
				}
			}
		}

		for(rapidxml::xml_node<>* creature = element->first_node("creature"); creature; creature = creature->next_sibling("creature")) {
			for(std::list<Creature*>::iterator it=game->freeCreatures.begin(); it != game->freeCreatures.end(); ++it) {
				if((*it)->name == std::string(creature->value())) {
					newRoom->creatures.push_back(*it);
					game->freeCreatures.remove(*it);
					break;
				}
			}
		}
		game->rooms.push_back(newRoom);
	}

	for(std::list<Room*>::iterator it = game->rooms.begin(); it != game->rooms.end(); ++it) {
		if((*it)->name == "Entrance") {
			game->currRoom = (*it);
			break;
		}
	}

	return game;
}

void rungame(gameObject* game) {
	std::string line;
	std::cout << game->currRoom->description << "\n";
	while(std::cin) {
		std::getline(std::cin, line);
		//check triggers override
		//if not execute command
		if(execute_command(game, line)) {
			break;
		}
		//check activated triggers, execute actions and check additional triggers

	}
}

bool execute_command(gameObject* game, std::string command) {
	if(command == "open exit" && game->currRoom->type) {
		std::cout << "Game Over\n";
		return true;
	} else if(command == "force quit") {
		return true;;
	}

	bool force = false;
	if(command == "n") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "north") {
				game->currRoom = searchRoom(&(game->rooms), (*it).name);
				std::cout << game->currRoom->description << std::endl;
				force = true;
				break;
			}
		}
		if(force == false) {
			std::cout << "Can't go that way\n";
		}
		return false;
	} else if(command == "s") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "south") {
				game->currRoom = searchRoom(&(game->rooms), (*it).name);
				std::cout << game->currRoom->description << std::endl;
				force = true;
				break;
			}
		}
		if(force == false) {
			std::cout << "Can't go that way\n";
		}
		return false;
	} else if(command == "e") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "east") {
				game->currRoom = searchRoom(&(game->rooms), (*it).name);
				std::cout << game->currRoom->description << std::endl;
				force = true;
				break;
			}
		}
		if(force == false) {
			std::cout << "Can't go that way\n";
		}
		return false;
	} else if(command  == "w") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "west") {
				game->currRoom = searchRoom(&(game->rooms), (*it).name);
				std::cout << game->currRoom->description << std::endl;
				force = true;
				break;
			}
		}
		if(force == false) {
			std::cout << "Can't go that way\n";
		}
		return false;
	} else if(command == "i") {
		if(game->inventory.items.size() == 0) {
			std::cout << "Inventory: empty\n";
		} else {
			std::cout << "Inventory: " << game->inventory.items.front()->name;
			std::list<Item*>::iterator it = game->inventory.items.begin();
			for(++it; it != game->inventory.items.end(); ++it) {
				std::cout << ", " << (*it)->name;
			}
			std::cout << std::endl;
		}
		return false;
	}

	return false;
}

Room* searchRoom(std::list<Room*>* rooms, std::string roomName) {
	for(std::list<Room*>::iterator it = (*rooms).begin(); it != (*rooms).end(); ++it) {
		if((*it)->name == roomName) {
			return (*it);
		}
	}
	return NULL;
}

Item* searchItem(std::list<Item*>* items, std::string itemName) {
	for(std::list<Item*>::iterator it = (*items).begin(); it != (*items).end(); ++it) {
		if((*it)->name == itemName) {
			return (*it);
		}
	}
	return NULL;
}

Container* searchContainer(std::list<Container*>* containers, std::string containerName) {
	for(std::list<Container*>::iterator it = (*containers).begin(); it != (*containers).end(); ++it) {
		if((*it)->name == containerName) {
			return (*it);
		}
	}
	return NULL;
}

Creature* searchCreature(std::list<Creature*>* creatures, std::string creatureName) {
	for(std::list<Creature*>::iterator it = (*creatures).begin(); it != (*creatures).end(); ++it) {
		if((*it)->name == creatureName) {
			return (*it);
		}
	}
	return NULL;
}
