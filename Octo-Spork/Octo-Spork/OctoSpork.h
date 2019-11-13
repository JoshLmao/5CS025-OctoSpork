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
#include "rapidjson\document.h"

class OctoSpork
{
public:
	OctoSpork();

	/* Entry point - Call to start game */
	void PlayGame();

	const Colors ITEMS_COLOR = Colors::COLORS_GREEN;
	const Colors NPCS_COLOR = Colors::COLORS_CYAN;
	const Colors EXITS_COLOR = Colors::COLORS_MAGENTA;
	const Colors DEFAULT_COLOR = Colors::COLORS_WHITE;

private:
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

	/* Displays help for using Octo-Spork, shows all accepted commands */
	void Help(Input::Instruction instruction);
	/* Talks to a NPC in the current room */
	void Talk(Input::Instruction instruction);
	/* Enters a new room through one of the current room's exits */
	void Enter(Input::Instruction instruction);
	/* Gives an item to the current NPC player is speaking with */
	void Give(Input::Instruction instruction);
	/* Takes an item from the current room */
	void Take(Input::Instruction instruction);
	/* Drops an item from the users inventory if they have it */
	void Drop(Input::Instruction instruction);
	/* Examines an item in the user's inventory */
	void Examine(Input::Instruction instruction);

	/* Adds the final two rooms required to finish the game */
	void ActivateFinalRooms();

	/* Reads a text file and returns all content */
	std::string ReadFile(std::string filename);
	/* Parses game data json and populates ready to play */
	void ParseJSON(const char* json);

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