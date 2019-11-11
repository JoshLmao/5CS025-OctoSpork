#pragma once
#include <vector>
#include <string>

#include "Function.h"
#include "Direction.h"
#include "Item.h"
#include "Room.h"
#include "Input.h"
#include "Utils.h"
#include "Inventory.h"

class OctoSpork
{
public:
	OctoSpork();

	/* Entry point - Call to start game */
	void PlayGame();

private:
	// Lightning stone name
	const std::string LIGHTNING_STONE = "Lightning Stone";
	const std::string LIGHTNING_STONE_DESCRIPTION = "A special stone, filled with lightning. I should hold onto this...";
	// Earth stone name
	const std::string EARTH_STONE = "Earth Stone";
	const std::string EARTH_STONE_DESCRIPTION = "A special stone, infused with the elements of a far away planet.";
	// Light stone name
	const std::string LIGHT_STONE = "Light Stone";
	const std::string LIGHT_STONE_DESCRIPTION = "A special stone, encased with protective casing to contain a bright, glowing light inside";

	// All rooms containing their items
	std::vector<Room*> m_allRooms;
	// The user's inventory
	Inventory m_userInventory;
	// Current index of the room the user is in
	int m_roomIndex;
	// Is the user currently playing the game
	bool m_playingGame;
	// Ptr to the current NPC player is talking to
	NPC* m_currentNPCPtr;

	/* Initializes the game */
	void Init();
	/* Plays the game, enters the game loop */
	void Play();
	/* Disposes of all created objects ready for shutdown */
	void Dispose();

	/* Adds an amount of new line char's specified in 'count' */
	void InfoBuffer(int count = 1);
	/* Displays a message to the console with a specific format */
	inline void DisplayInfo(std::string msg);
	/* Sleeps the main thread for the specified amount of milliseconds */
	inline void Sleep(int ms);

	/* Updates the game from the urser's instruction */
	void UpdateState(Input::Instruction usrInstruction);
	/*	Prints out room information in the format:
		> Room Name
		> Room Description
		> - - - -
		> Items: Itm1, Itm2
		> Exits: Exit1, Exit2
		> NPCs: NPC1, NPC2
		*/
	void PrintRoomInfo(Room& r);
	/*	Prints out the inventory of the user */
	void PrintInventory();

	/* Checks if all Spirits have their required item to proceed to ending */
	bool SpiritsHaveItems();
	void TryTakeItem(std::string itemName);
	/* Finds the index of the requested room and returns it. Returns -1 if not found */
	int FindRoomIndex(std::string roomName);
};