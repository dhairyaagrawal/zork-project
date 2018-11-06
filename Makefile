project1 : project1.cpp TurnOn.cpp Owner.cpp Status.cpp Trigger.cpp Item.cpp Base.cpp Creature.cpp Attack.cpp Container.cpp Room.cpp Border.cpp gameObject.cpp
	@g++ project1.cpp TurnOn.cpp Owner.cpp Status.cpp Trigger.cpp Item.cpp Base.cpp Creature.cpp Attack.cpp Container.cpp Room.cpp Border.cpp gameObject.cpp -o IPA1

clean: IPA1
	@rm IPA1
