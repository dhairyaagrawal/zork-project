README.txt
 Created on: Oct 27, 2018
     Author: Dhairya

Zork game created in C++. Takes map of the game as an xml file and runs comman line input as commands to play the game.

Assumed Specifications (because of incomplete details):
	- Triggers can have multiple conditions but only one of each type (command, owner, status)
	- Attack can only one have one condition of type status and none of type owner
	- Commands have to matched completely for the trigger condition to pass because matching elements can lead
	  to wrong commands matching
	- Every item can only be created once; if you add an item to a container, it is moved
	  from the original container to the new one
	- Running the executable would modify the input file by putting null terminators in the source text
	- Rooms can only be of type regular or exit
	- Triggers can only be of type once or permanent
	- Triggers are by default of type once unless specified to permanent
	- Room are by default of type regular unless specified to be exit
	- Two different objects cannot have same name
	
