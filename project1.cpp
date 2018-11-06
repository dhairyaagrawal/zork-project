//============================================================================
// Name        : project.cpp
// Author      : Dhairya
// Version     :
// Copyright   : Your copyright notice
// Description : Zork Project in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>

#include <sstream>
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
bool checkRunTriggers(std::list<Trigger*>*, gameObject*, std::string);

Base* searchBase(std::list<Base*>*, std::string);
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
			if(element->first_node("turnon")->first_node("print")) {
				newItem->turnon.print = element->first_node("turnon")->first_node("print")->value();
			}
			if(element->first_node("turnon")->first_node("action")) {
				newItem->turnon.action = element->first_node("turnon")->first_node("action")->value();
			}
		}

		for(rapidxml::xml_node<>* trigger = element->first_node("trigger"); trigger; trigger = trigger->next_sibling("trigger")) {
			Trigger* newTrigger = new Trigger();
			newTrigger->command = trigger->first_node("command") ? trigger->first_node("command")->value() : "";
			if(trigger->first_node("type")) {
				newTrigger->type = (std::string(trigger->first_node("type")->value()) == "permanent") ? true : false;
			}
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
		game->allObjects.push_back(newItem);
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
			if(trigger->first_node("type")) {
				newTrigger->type = (std::string(trigger->first_node("type")->value()) == "permanent") ? true : false;
			}
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
		game->allObjects.push_back(newCreature);
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
			if(trigger->first_node("type")) {
				newTrigger->type = (std::string(trigger->first_node("type")->value()) == "permanent") ? true : false;
			}
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
		game->allObjects.push_back(newContainer);
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
			if(trigger->first_node("type")) {
				newTrigger->type = (std::string(trigger->first_node("type")->value()) == "permanent") ? true : false;
			}
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
		game->allObjects.push_back(newRoom);
	}

	for(std::list<Room*>::iterator it = game->rooms.begin(); it != game->rooms.end(); ++it) {
		if((*it)->name == "Entrance") {
			game->currRoom = (*it);
			break;
		}
	}
	game->allObjects.push_back(static_cast<Base*>(&game->inventory));
	return game;
}

void rungame(gameObject* game) {
	std::string line;
	std::cout << game->currRoom->description << "\n";
	while(std::cin) {
		std::getline(std::cin, line);
		//check triggers override
		bool run = false;
		bool tmp;
		run = checkRunTriggers(&(game->currRoom->triggers), game, line);
		for(std::list<Creature*>::iterator it = game->currRoom->creatures.begin(); it != game->currRoom->creatures.end(); ++it) {
			tmp = checkRunTriggers(&((*it)->triggers), game, line);
			if(tmp) {run = tmp;}
		}
		for(std::list<Container*>::iterator it = game->currRoom->containers.begin(); it != game->currRoom->containers.end(); ++it) {
			tmp = checkRunTriggers(&((*it)->triggers), game, line);
			if(tmp) {run = tmp;}
		}
		for(std::list<Item*>::iterator it = game->currRoom->items.begin(); it != game->currRoom->items.end(); ++it) {
			tmp = checkRunTriggers(&((*it)->triggers), game, line);
			if(tmp) {run = tmp;}
		}

		//if not execute command
		if(!run) {
			run = true;
			if(execute_command(game, line)) {
				break;
			}
		} else {
			run = false;
		}

		//check activated triggers, execute actions and check additional triggers
		while(run) {
			run = false;
			run = checkRunTriggers(&(game->currRoom->triggers), game, "");
			for(std::list<Creature*>::iterator it = game->currRoom->creatures.begin(); it != game->currRoom->creatures.end(); ++it) {
				tmp = checkRunTriggers(&((*it)->triggers), game, "");
				if(tmp) {run = tmp;}
			}
			for(std::list<Container*>::iterator it = game->currRoom->containers.begin(); it != game->currRoom->containers.end(); ++it) {
				tmp = checkRunTriggers(&((*it)->triggers), game, "");
				if(tmp) {run = tmp;}
			}
			for(std::list<Item*>::iterator it = game->currRoom->items.begin(); it != game->currRoom->items.end(); ++it) {
				tmp = checkRunTriggers(&((*it)->triggers), game, "");
				if(tmp) {run = tmp;}
			}
		}
	}
}

bool execute_command(gameObject* game, std::string command) {
	if(command == "open exit" && game->currRoom->type) {
		std::cout << "Game Over\n";
		return true;
	} else if(command == "open exit") {
		std::cout << "Room not of type exit\n";
		return false;
	} else if(command == "force quit") {
		return true;;
	} else if(command == "Game Over") {
		std::cout << "Victory!\n";
		delete game;
		std::exit(0);
		return true;
	 }

	bool force = false;
	if(command == "n") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "north") {
				Room* tmp = searchRoom(&(game->rooms), (*it).name);
				if(tmp) {
					game->currRoom = tmp;
					std::cout << game->currRoom->description << std::endl;
					force = true;
					break;
				}
			}
		}
		if(force == false) {
			std::cout << "Can't go that way\n";
		}
		return false;
	} else if(command == "s") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "south") {
				Room* tmp = searchRoom(&(game->rooms), (*it).name);
				if(tmp) {
					game->currRoom = tmp;
					std::cout << game->currRoom->description << std::endl;
					force = true;
					break;
				}
			}
		}
		if(force == false) {
			std::cout << "Can't go that way\n";
		}
		return false;
	} else if(command == "e") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "east") {
				Room* tmp = searchRoom(&(game->rooms), (*it).name);
				if(tmp) {
					game->currRoom = tmp;
					std::cout << game->currRoom->description << std::endl;
					force = true;
					break;
				}
			}
		}
		if(force == false) {
			std::cout << "Can't go that way\n";
		}
		return false;
	} else if(command  == "w") {
		for(std::list<Border>::iterator it = game->currRoom->borders.begin(); it != game->currRoom->borders.end(); ++it) {
			if((*it).direction == "west") {
				Room* tmp = searchRoom(&(game->rooms), (*it).name);
				if(tmp) {
					game->currRoom = tmp;
					std::cout << game->currRoom->description << std::endl;
					force = true;
					break;
				}
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
			for(++it; it != game->inventory.items.end(); ++it) { //?????
				std::cout << ", " << (*it)->name;
			}
			std::cout << std::endl;
		}
		return false;
	}
	
	std::list<std::string> newCommand;
	std::istringstream iss (command);
	for(std::string s = ""; iss >> s; ) {
    	newCommand.push_back(s);
	}

	if(newCommand.front() == "open" && newCommand.size() == 2) {
		newCommand.pop_front();
		Container* target = searchContainer(&(game->currRoom->containers), newCommand.front());
		if(!target) {
			std::cout << newCommand.front() << " container not present\n";
		} else if(target->accept.size() == 0 || target->status == "unlocked") {
			target->open = true;
			target->printItems();
		} else {
			std::cout << "Can't open " << target->name << std::endl;
		}
		return false;
	} else if(newCommand.front() == "take" && newCommand.size() == 2) {
		newCommand.pop_front();
		Item* target = searchItem(&(game->currRoom->items), newCommand.front());
		if(target) {
			game->inventory.items.push_back(target);
			game->currRoom->items.remove(target);
			std::cout << "Item " << target->name << " added to inventory\n";
		} else {
			for(std::list<Container*>::iterator it = game->currRoom->containers.begin(); it != game->currRoom->containers.end(); ++it) {
				target = searchItem(&((*it)->items), newCommand.front());
				if(target) {
					if((*it)->open) {
						game->inventory.items.push_back(target);
						(*it)->items.remove(target);
						std::cout << "Item " << target->name << " added to inventory\n";
					} else {
						std::cout << (*it)->name << " not open\n";
					}
					break;
				}
			}
			if(!target) {
				std::cout << "Item not found\n";
			}
		}
		return false;
	} else if(newCommand.front() == "read" && newCommand.size() == 2) {
		newCommand.pop_front();
		Item* target = searchItem(&(game->inventory.items), newCommand.front());
		if(target) {
			if(target->writing == "") {
				std::cout << "Nothing written\n";
			} else {
				std::cout << target->writing << std::endl;
			}
		} else {
			std::cout << "You don't have " << newCommand.front() << " in inventory\n";
		}
		return false;
	} else if(newCommand.front() == "drop" && newCommand.size() == 2) {
		newCommand.pop_front();
		Item* target = searchItem(&(game->inventory.items), newCommand.front());
		if(target) {
			game->currRoom->items.push_back(target);
			game->inventory.items.remove(target);
			std::cout << target->name << " dropped\n";
		} else {
			std::cout << "You don't have " << newCommand.front() << " in inventory\n";
		}
		return false;
	} else if(newCommand.front() == "put" && newCommand.size() == 4) {
		newCommand.pop_front();
		Item* target = searchItem(&(game->inventory.items), newCommand.front());
		if(target) {
			newCommand.pop_front();
			newCommand.pop_front();
			Container* targetContainer = searchContainer(&(game->currRoom->containers), newCommand.front());
			if(targetContainer && (targetContainer->accept.size() == 0)) {
				targetContainer->items.push_back(target);
				game->inventory.items.remove(target);
				std::cout << "Item " << target->name << " added to " << targetContainer->name << "\n";
			} else if(targetContainer && (targetContainer->accept.size() != 0)) {
				bool force = false;
				for(std::list<std::string>::iterator it = targetContainer->accept.begin(); it != targetContainer->accept.end(); ++it) {
					if(target->name == (*it)) {
						targetContainer->items.push_back(target);
						game->inventory.items.remove(target);
						targetContainer->accept.remove(*it); //removing accepting element
						std::cout << "Item " << target->name << " added to " << targetContainer->name << "\n";
						force = true;
						break;
					}
				}
				if(!force) {
					std::cout << "Item " << target->name << " cannot be added to " << targetContainer->name << "\n";
				}
			} else {
				std::cout << "Container " << newCommand.front() << " not found in current room\n";
			}
		} else {
			std::cout << "You don't have " << newCommand.front() << " in inventory\n";
		}
		return false;
	} else if(newCommand.front() == "Update" && newCommand.size() == 4) {
		newCommand.pop_front();
		Base* target = searchBase(&(game->allObjects), newCommand.front());
		if(target) {
			newCommand.pop_front();
			newCommand.pop_front();
			target->status = newCommand.front();
		} else {
			std::cout << "Object " << newCommand.front() << " not found\n";
		}
		return false;
	} else if(newCommand.front() == "Delete" && newCommand.size() == 2) {
		newCommand.pop_front();
		Base* target = searchBase(&(game->allObjects), newCommand.front());
		Item* notIn = searchItem(&(game->inventory.items), newCommand.front());
		if(target && (notIn == NULL)) {
			if(target->derivedType == "Room") {
				game->rooms.remove(static_cast<Room*>(target));
				//std::cout << "Room " << target->name << " removed\n";
				delete target;
			} else if(target->derivedType == "Container") {
				for(std::list<Room*>::iterator it = game->rooms.begin(); it != game->rooms.end(); ++it) {
					Container* newTarget = searchContainer(&((*it)->containers), target->name);
					if(newTarget) {
						(*it)->containers.remove(static_cast<Container*>(target));
						game->freeContainers.push_back(static_cast<Container*>(target));
						//std::cout << "Container " << target->name << " removed\n";
						break;
					}
				}
			} else if(target->derivedType == "Creature") {
				for(std::list<Room*>::iterator it = game->rooms.begin(); it != game->rooms.end(); ++it) {
					Creature* newTarget = searchCreature(&((*it)->creatures), target->name);
					if(newTarget) {
						(*it)->creatures.remove(static_cast<Creature*>(target));
						game->freeCreatures.push_back(static_cast<Creature*>(target));
						//std::cout << "Creature " << target->name << " removed\n";
						break;
					}
				}
			} else if(target->derivedType == "Item") {
				for(std::list<Room*>::iterator it = game->rooms.begin(); it != game->rooms.end(); ++it) {
					Item* newTarget = searchItem(&((*it)->items), target->name);
					if(newTarget) {
						(*it)->items.remove(static_cast<Item*>(target));
						game->freeItems.push_back(static_cast<Item*>(target));
						//std::cout << "Item " << target->name << " removed\n";
						break;
					} else {
						bool force = false;
						for(std::list<Container*>::iterator newIt = (*it)->containers.begin(); newIt != (*it)->containers.end(); ++it) {
							newTarget = searchItem(&((*newIt)->items), target->name);
							if(newTarget) {
								(*newIt)->items.remove(static_cast<Item*>(target));
								game->freeItems.push_back(static_cast<Item*>(target));
								//std::cout << "Item " << target->name << " removed\n";
								force = true;
								break;
							}

						}
						if(force) {break;}
					}
				}
			}
		} else {
			std::cout << "Object " << newCommand.front() << " not found\n";
		}
		return false;
	} else if(newCommand.front() == "Add" && newCommand.size() == 4) {
		newCommand.pop_front();
		Base* target = searchBase(&(game->allObjects), newCommand.front());
		if(target) {
			newCommand.pop_front();
			newCommand.pop_front();
			if(target->derivedType == "Creature") {
				Room* room = searchRoom(&(game->rooms), newCommand.front());
				if(room == NULL) {std::cout << "Room not found\n"; return false;}
				room->creatures.push_back(static_cast<Creature*>(target));
				game->freeCreatures.remove(static_cast<Creature*>(target));
			} else if(target->derivedType == "Container") {
				Room* room = searchRoom(&(game->rooms), newCommand.front());
				if(room == NULL) {std::cout << "Room not found\n"; return false;}
				room->containers.push_back(static_cast<Container*>(target));
				game->freeContainers.remove(static_cast<Container*>(target));
			} else if(target->derivedType == "Item") {
				Room* room = searchRoom(&(game->rooms), newCommand.front());
				if(room != NULL) {
					room->items.push_back(static_cast<Item*>(target));
					game->freeItems.remove(static_cast<Item*>(target));
				} else {
					Base* container = searchBase(&(game->allObjects), newCommand.front());
					if(container) {
						static_cast<Container*>(container)->items.push_back(static_cast<Item*>(target));
						game->freeItems.remove(static_cast<Item*>(target));
					} else {
						std::cout << "Room/Container not found to Add\n";
					}
				}
			} else {
				std::cout << "Can't perform Add\n";
			}
		} else {
			std::cout << "Object " << newCommand.front() << " not found\n";
		}
		return false;
	} else if(newCommand.front() == "turn") {
		newCommand.pop_front();
		if(newCommand.front() != "on") {std::cout << "Command Error\n"; return false;}
		newCommand.pop_front();
		Item* target = searchItem(&(game->inventory.items), newCommand.front());
		if(target) {
			if(target->turnon.exists) {
				std::cout << target->turnon.print << std::endl;
				bool val;
				val = execute_command(game, target->turnon.action);
				std::cout << "You activate the " << target->name << "\n";
				return val;
			} else {
				std::cout << "Item can't be turned on\n";
			}
		} else {
			std::cout << "Item not found\n";
		}
		return false;
	} else if(newCommand.front() == "attack") {
		newCommand.pop_front();
		Creature* creature = searchCreature(&(game->currRoom->creatures), newCommand.front());
		if(creature == NULL) {std::cout << "Creature not in room\n"; return false;}
		newCommand.pop_front();
		if(newCommand.front() != "with") {std::cout << "Command Error\n"; return false;}
		newCommand.pop_front();
		Item* item = searchItem(&(game->inventory.items), newCommand.front());
		if(item == NULL) {std::cout << "You don't have the item\n"; return false;}
		std::cout << "You assault the " << creature->name << " with the " << item->name << "\n";
		if((creature->attack.statusCond.objName == item->name && creature->attack.statusCond.status == item->status) || creature->attack.statusCond.objName != item->name) {
			bool found = false;
			for(std::list<std::string>::iterator it = creature->weaknesses.begin(); it != creature->weaknesses.end(); ++it) {
				if((*it) == item->name) {found = true; break;}
			}
			if(!found) {std::cout << "unsuccessful attack\n"; return false;}
			for(std::list<std::string>::iterator it = creature->attack.prints.begin(); it != creature->attack.prints.end(); ++it) {
				std::cout << (*it) << std::endl;
			}
			found = false;
			for(std::list<std::string>::iterator it = creature->attack.actions.begin(); it != creature->attack.actions.end(); ++it) {
				found = execute_command(game, (*it));
			}
			return found;
		} else {
			std::cout << "unsuccessful attack\n";
			return false;
		}
	}

	std::cout << "Command Error\n";
	return false;
}

bool checkRunTriggers(std::list<Trigger*>* triggers, gameObject* game, std::string command) {
	bool pass = false;
	for(std::list<Trigger*>::iterator it = (*triggers).begin(); it != (*triggers).end(); ++it) {
		if(((*it)->type == false && (*it)->used == false) || (*it)->type) {}
		else {continue;}
		if((*it)->command != "" && command != (*it)->command) {continue;}
		if((*it)->ownerCond.objName != "") {
			Base* owner = searchBase(&(game->allObjects), (*it)->ownerCond.owner); //container
			if(owner && owner->derivedType == "Container") {
				Item* item = searchItem(&(static_cast<Container*>(owner)->items), (*it)->ownerCond.objName); //object
				if(((*it)->ownerCond.has && item) || (item == NULL && !((*it)->ownerCond.has))) {}
				else {continue;}
			} else if(owner->derivedType == "Room") {
				Item* item = searchItem(&(static_cast<Room*>(owner)->items), (*it)->ownerCond.objName); //object
				Creature* creature = searchCreature(&(static_cast<Room*>(owner)->creatures), (*it)->ownerCond.objName); //object
				if(((*it)->ownerCond.has && item) || (item == NULL && !((*it)->ownerCond.has))) {}
				else if(((*it)->ownerCond.has && creature) || (creature == NULL && !((*it)->ownerCond.has))) {}
				else {continue;}
			}
		}
		if((*it)->statusCond.objName != "") {
			Base* obj = searchBase(&(game->allObjects), (*it)->statusCond.objName);
			if(obj->status == (*it)->statusCond.status) {}
			else {continue;}
		}
		for(std::list<std::string>::iterator tmp = (*it)->prints.begin(); tmp != (*it)->prints.end(); ++tmp) {
			std::cout << (*tmp) << std::endl;
		}
		for(std::list<std::string>::iterator tmp = (*it)->actions.begin(); tmp != (*it)->actions.end(); ++tmp) {
			bool ret = execute_command(game, (*tmp));
		}
		(*it)->used = true;
		pass = true;
	}
	return pass;
}

Base* searchBase(std::list<Base*>* bases, std::string name) {
	for(std::list<Base*>::iterator it = (*bases).begin(); it != (*bases).end(); ++it) {
		if((*it)->name == name) {return (*it);}
	}
	return NULL;
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
