#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <vector>
#include <thread>
#include <chrono>

#include "Function.h"
#include "Direction.h"
#include "Item.h"
#include "Room.h"
#include "Input.h"
#include "Utils.h"
#include "Inventory.h"

void DisplayInfo(std::string message);
void InitBuilding();
void InfoBuffer(int count = 1);
void PlayGame();
void UpdateState(Input::Instruction usrInstruction);
int FindRoomIndex(std::string roomName);
void PrintRoomInfo(Room & r);
void PrintInventory();
void Sleep(int ms);
void DisposeGame();
bool SpiritsHaveItems();
void TryTakeItem(std::string itmName);

const int ROOM_COUNT = 11;
// Names of items required to finish the game
const std::string LIGHTNING_STONE = "Lightning Stone";
const std::string EARTH_STONE = "Earth Stone";
const std::string LIGHT_STONE = "Light Stone";

/// All rooms containing their items
static std::vector<Room*> AllRooms;
/// The user's inventory
Inventory m_userInventory;
/// Current index of the room the user is in
int m_roomIndex = 0;
/// Is the user currently playing the game
bool m_playingGame = false;
NPC* m_currentNPCPtr = nullptr;

int main(int arc, char* argv[])
{
	// Initialize all rooms, items before starting any gameplay
	InitBuilding();

	PlayGame();
	return 0;
}

/* Begin playing Octo-Spork, main entry */
void PlayGame()
{
	m_playingGame = true;
	DisplayInfo("Welcome to Octo-Spork!");
	DisplayInfo("Created by Josh Shepherd (1700471)");
	InfoBuffer(2);

	Sleep(1000);

	// Introduction
	DisplayInfo(" - Introduction - ");
	Sleep(2000);
	DisplayInfo("You awaken on the floor of a cold, damp and dilapidated structure. You're feeling dazed, a little confused with a slight pain in the back of your head");
	Sleep(4000);
	DisplayInfo("As you stand up, you notice you're stood inside a massive, abandoned facility. Scanning the room, you see a sign that says 'Main Hall'");
	Sleep(4000);
	DisplayInfo("A massive wall of endless running cloud falls down one side of the room. It seems to be blocking a path to somewhere");
	Sleep(4000);
	DisplayInfo("You also notice there are three ghostly figures floating up and down in front of the cloud wall. Maybe they can shed some light on how you got here and why?");

	InfoBuffer(2);
	
	PrintRoomInfo(*AllRooms[m_roomIndex]);
	InfoBuffer();
	while (m_playingGame)
	{
		DisplayInfo("What will you do?");
		Input::Instruction inst = Input::ReadUser();
		InfoBuffer();

		if (inst.Function != FUNCTION_NONE) {
			UpdateState(inst);
		}
		else {
			DisplayInfo("Invalid command. Try again");
		}
		
		InfoBuffer();
	}

	DisplayInfo("Thanks for playing! Hope you enjoyed the journey");
}

/* Creates the buildings rooms, items, etc */
void InitBuilding()
{
	std::vector<std::string> exits;

	// Spawn Room, Main Hall
	exits = { "North Hallway", "Staircase" };
	Room* mainHall = new Room("Main Hall", "Where I woke up. There's a massive sign on the wall with it's name", exits);

#if _DEBUG
	// Here's your cheats, ya filthy animal
	mainHall->AddItem(new Item(LIGHTNING_STONE));
	mainHall->AddItem(new Item(EARTH_STONE));
	mainHall->AddItem(new Item(LIGHT_STONE));
#endif

	// One of Three Spitis, Zeus
	NPCConfig zeusConfig = NPCConfig();
	zeusConfig.Name = "Spirit of Zeus";
	zeusConfig.RequiredItemName = LIGHTNING_STONE;
	zeusConfig.Greeting = "Hello there, wanderer. I see you finally awaken from your slumber. Ready yourself, soon Odin will return to serve his punishment";
	zeusConfig.StandardResponse = "Odin creeps ever closer. Your time here is limited. I hope this doesn't come as a 'shock' to you. Mwuahahah";
	zeusConfig.ExcessiveLimitCount = 6;
	zeusConfig.ExcessiveResponse = "STOP annoying me with your small talk! Do you require anything from me?";
	zeusConfig.HasItemResponse = "Wait. No. Could it be? How did you find this? Give it here, quickly.";
	zeusConfig.CompleteResponse = "If you find any other stones... maybe we will be lenient when Odin arrives";
	zeusConfig.IncorrectItemResponse = "Why are you giving me this? Ugh, stop wasting my time. Odin will strike swiftly, once he arrives";
	NPC* zeusNPC = new NPC(zeusConfig);
	mainHall->AddNPC(zeusNPC);

	// One of Three Spitis, Freya
	NPCConfig freyaConfig = NPCConfig();
	freyaConfig.Name = "Spirit of Freya";
	freyaConfig.RequiredItemName = EARTH_STONE;
	freyaConfig.Greeting = "Greetings, my child. My deepest sympothies about your situation. I wish I could do something to help, but Odin arrives soon.";
	freyaConfig.StandardResponse = "I hope that you can find peace with yourself in your final hours. Your situation is a complicated one";
	freyaConfig.ExcessiveLimitCount = 15;
	freyaConfig.ExcessiveResponse = "Please stop begging me. There is nothing more I can do for you.";
	freyaConfig.HasItemResponse = "Oh my... How did you get this?! Please hand it over. I hope for your sake that there are more for you to find";
	freyaConfig.CompleteResponse = "Have a look around. Zeus & Baulder could be persuaded if you could do the same for them";
	freyaConfig.IncorrectItemResponse = "No, I don't need this.";
	NPC* freyaNPC = new NPC(freyaConfig);
	mainHall->AddNPC(freyaNPC);

	// One of Three Spitis, Baulder
	NPCConfig baulderConfig = NPCConfig();
	baulderConfig.Name = "Spirit of Baulder";
	baulderConfig.RequiredItemName = LIGHT_STONE;
	baulderConfig.Greeting = "Well, well, well. The little brat is finally awake. What's the matter? Scared of Odin's punishment? Hah, you should be. May the guardian light shine through your final hours";
	baulderConfig.StandardResponse = "Whatd'ya want? Come to grovel to me? It 'aint gonna work.";
	baulderConfig.ExcessiveLimitCount = 8;
	baulderConfig.ExcessiveResponse = "Look, you 'aint getting any help from me. Go be annoying somewhere else.";
	baulderConfig.HasItemResponse = "Oh, how shiny! Pass it 'ere.";
	baulderConfig.CompleteResponse = "Yeah? What is it?";
	baulderConfig.IncorrectItemResponse = "Is this meant to impress me? Well congratulations... *clap* *clap* *clap* - it didn't.";
	NPC* baulderNPC = new NPC(baulderConfig);
	mainHall->AddNPC(baulderNPC);

	// Ground Floor - Hallways
	exits = { "Main Hall", "East Hallway", "MI034" };
	Room* nHallway = new Room("North Hallway", "Long hallway", exits);
	
	exits = { "North Hallway", "MI035" };
	Room* eHallway = new Room("East Hallway", "Hidden in the back of the complex", exits);

	// Ground Floor - Rooms
	// MI034
	exits = { "North Hallway", "MI035" };
	Room* mi034 = new Room("MI034", "A quiet little room tucked away at the back of the facility", exits);
	mi034->AddItem(new Item("Computer Mouse"));
	mi034->AddItem(new Item("Broken Headset"));
	
	NPCConfig govrConfig = NPCConfig();
	govrConfig.Name = "Ghost of VR";
	govrConfig.RequiredItemName = "Example Item";
	govrConfig.Greeting = "Greetings wanderer. What brings you along my path? Is it the promise of loot or to free me from this cursed room?";
	govrConfig.StandardResponse = "Please, wanderer. I have been here for 500 years now. My death is the only thing that bounds me to the realm. Can you help free me?";
	govrConfig.ExcessiveResponse = "Do you wish to torture me by returning so often? Help me or leave me be.";
	govrConfig.HasItemResponse = "Ah, finally! Now I may rest in peace. I am eternally greateful. Thank you. And here, take this I found lying around";
	govrConfig.CompleteResponse = "You have served me well. Now go, let me play VR.";
	govrConfig.IncorrectItemResponse = "What is this? Don't waste my time with irrelevent things.";
	govrConfig.ExcessiveLimitCount = 15;

	NPC* ghostVR = new NPC(govrConfig);
	Item* stone = new Item(LIGHT_STONE);
	ghostVR->SetReward(stone);

	mi034->AddNPC(ghostVR);

	// MI035
	exits = { "East Hallway" };
	Room* mi035 = new Room("MI035", "Description", exits);
	mi035->AddItem(new Item("Ominous Candle"));
	mi035->AddItem(new Item("VR Controller"));

	// Floor 1 to 2 Staircase 
	exits = { "Main Hall", "F2 Landing" };
	Room* staircase = new Room("Staircase", "", exits);

	// Floor 2
	exits = { "Staircase", "F2 North Hallway", "F2 West Hallway" };
	Room* f2Landing = new Room("F2 Landing", "A massive open space, full of old and rusted parts. From here I can see three room, all look abandoned and filled with broken glass", exits);

	exits = { "F2 East Hallway", "F2 Landing", "MI102c", "F2 West Hallway" };
	Room* f2nHallway = new Room("F2 North Hallway", "Another hallway that leads to more dark places", exits);

	exits = { "F2 North Hallway", "F2 East Hallway", "MI102c" };
	Room* f2eHallway = new Room("F2 East Hallway", "An almost enclosed space with complete darkness at the other end. I don't feel like staying here for longer than necessary...", exits);

	exits = { "F2 Landing", "F2 North Hallway", "MI102a", "MI102b" };
	Room* f2wHallway = new Room("F2 West Hallway", "Another hallway. A wall of rolling cloud fills the end of the hallway. Seems impassable", exits);

	// Floor 2 - Rooms
	exits = { "F2 Landing" };
	Room* mi102a = new Room("MI102a", "", exits);
	Item* earthStone = new Item(EARTH_STONE);
	mi102a->AddItem(earthStone);

	Room* mi102b = new Room("MI102b", "", exits);

	exits = { "F2 North Hallway" };
	Room* mi102c = new Room("MI102c", "", exits);

	// Add all rooms to Vector
	AllRooms.push_back(mainHall);
	AllRooms.push_back(nHallway);
	AllRooms.push_back(eHallway);
	AllRooms.push_back(mi034);
	AllRooms.push_back(mi035);
	AllRooms.push_back(staircase);
	AllRooms.push_back(f2Landing);
	AllRooms.push_back(f2nHallway);
	AllRooms.push_back(mi102a);
	AllRooms.push_back(mi102b);
	AllRooms.push_back(mi102c);
}

/* Adds an amount of new line char's specified in 'count' */
void InfoBuffer(int count)
{
	std::string el;
	for (int i = 0; i < count; i++) {
		el += "\n";
	}
	std::cout << el;
}

/* Displays a message to the console with a specific format */
inline void DisplayInfo(std::string message)
{
	std::cout << "> " << message << std::endl;
}

/* Sleeps the main thread for the specified amount of milliseconds */
inline void Sleep(int ms)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void UpdateState(Input::Instruction usrInstruction)
{
	if (usrInstruction.Function == Function::FUNCTION_HELP) 
	{
		// User asking for help
		DisplayInfo("Help Commands");
		DisplayInfo("help - Lists all available commands to use");
		DisplayInfo("enter 'ROOM' - Used to move from one room to another");
		DisplayInfo("talk 'NPC' - Talks to an NPC");
		DisplayInfo("give 'ITEM' - Attempts to give an item to the current NPC in the room");
		DisplayInfo("drop 'ITEM' - Drops the item in the current room");
		DisplayInfo("take 'ITEM' - Takes an item that is inside the current room");
		DisplayInfo("view inventory - Lists your current items you're holding");
		DisplayInfo("view room - Lists the current room's information");
		DisplayInfo("exit - Quit the game");

		InfoBuffer();
		DisplayInfo("Tip: Remember you can talk to NPCs multiple times for more information");
	}
	else if (usrInstruction.Function == Function::FUNCTION_TALK) 
	{
		// User talking to an NPC
		m_currentNPCPtr = AllRooms[m_roomIndex]->GetNPC(usrInstruction.Goal);
		
		if (m_currentNPCPtr == nullptr) {
			DisplayInfo("There is no one here named '" + usrInstruction.Goal + "'.");
		}
		else {
			std::string speech = m_currentNPCPtr->GetSpeech();
			DisplayInfo(speech);
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_ENTER)
	{
		// User enters new room
		for (int i = 0; i < AllRooms[m_roomIndex]->GetExitsSize(); i++)
		{
			std::string exitLower = Utils::ToLower(AllRooms[m_roomIndex]->GetExit(i));
			std::string goalLower = Utils::ToLower(usrInstruction.Goal);
			if (exitLower == goalLower)
			{
				m_roomIndex = FindRoomIndex(goalLower);
				break;
			}
		}

		// Reset current talking to NPC since user changed room
		if (m_currentNPCPtr != nullptr)
			m_currentNPCPtr = nullptr;

		// Print information of new room
		PrintRoomInfo(*AllRooms[m_roomIndex]);

		if (AllRooms[m_roomIndex]->Name == "?") {
			// End the game as player enter's the Unknown room

			DisplayInfo("As I enter the room, there is just... emptiness. A long, deafening silence.");
			Sleep(2500);
			DisplayInfo("...");
			Sleep(2500);
			DisplayInfo("...");
			Sleep(2500);
			DisplayInfo("...");
			
			Sleep(2500);
			DisplayInfo("A silhouette appears through the emptiness. A man, holding a spear in his right hand.");
			
			Sleep(2500);
			DisplayInfo("Is that.. a helmet with pointy horns?");

			Sleep(5000);
			DisplayInfo("It's Odin!");

			m_playingGame = false;
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_VIEW_INVENTORY) 
	{
		// User viewing their inventory
		DisplayInfo("You look through your belongings...");
		PrintInventory();
	}
	else if (usrInstruction.Function == Function::FUNCTION_VIEW_ROOM) {
		// Print the room info to the user
		PrintRoomInfo(*AllRooms[m_roomIndex]);
	}
	else if (usrInstruction.Function == Function::FUNCTION_GIVE) 
	{
		// User wants to give an item to the current NPC
		if (m_userInventory.Contains(usrInstruction.Goal)) {
			// Check if an NPC currently being spoken to
			if (m_currentNPCPtr == nullptr) {

				// Make user drop item since they're trying to GIVE before talking to NPC
				Item* itm = m_userInventory.RemoveItem(usrInstruction.Goal);
				AllRooms[m_roomIndex]->AddItem(itm);

				DisplayInfo("You try to give '" + itm->GetName() + "'. However, you only hear a loud clunk as the item falls to the floor.");
				DisplayInfo("Maybe you should talk to someone first before trying to give an item?");
				InfoBuffer();

				PrintRoomInfo(*AllRooms[m_roomIndex]);
				return;
			}

			bool completedNPC = m_currentNPCPtr->GiveItem(usrInstruction.Goal);
			DisplayInfo(m_currentNPCPtr->GetSpeech());

			if (completedNPC) {
				// Remove item from inventory, only if successful
				Item* itm = m_userInventory.RemoveItem(usrInstruction.Goal);
				delete itm;

				Item* rewardItm = m_currentNPCPtr->PopReward();
				if (rewardItm != nullptr) {
					// Buffer between speech of NPC and reward
					InfoBuffer();
					DisplayInfo("'" + m_currentNPCPtr->GetName() + "' rewards you with '" + rewardItm->GetName() + "'");

					m_userInventory.AddItem(rewardItm, true);
				}
			}
		} else {
			DisplayInfo("You don't have '" + usrInstruction.Goal + "' in your inventory");
		}

		if (SpiritsHaveItems()) 
		{
			// ToDo: Implement Ending
			DisplayInfo("The three spirits suddenly vanish. The endless cloud wall seems to fade away, revealing a series of doors, all leading to the same path");
			
			// Create room after cloud wall
			std::vector<std::string> exits = { "Main Hall", "?" };
			Room* finalRoom = new Room("Realm Beyond Realms", "The cloud wall revealed this room. This must lead to a way out! Surely...", exits);
			Item* itm = new Item("Dust Pile");
			finalRoom->AddItem(itm);
			AllRooms.push_back(finalRoom);
			
			// Add new room as exit in Main Hall
			AllRooms[0]->AddExit(finalRoom->Name);

			// Create room after Realm Beyond Realms, "?"
			exits = { };
			Room* unknown = new Room("?", "There's just... nothing.", exits);
			AllRooms.push_back(unknown);

			PrintRoomInfo(*AllRooms[m_roomIndex]);
			InfoBuffer();
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_TAKE) 
	{
		// User picks up an item. 
		int size = AllRooms[m_roomIndex]->GetItemsSize();
		if (size <= 0) {
			DisplayInfo("There are no items here.");
			return;
		}

		TryTakeItem(usrInstruction.Goal);
	}
	else if(usrInstruction.Function == Function::FUNCTION_DROP)
	{
		// User wants to drop an item
		bool contains = m_userInventory.Contains(usrInstruction.Goal);
		if (contains) {
			Item* itmPtr = m_userInventory.RemoveItem(usrInstruction.Goal);
			AllRooms[m_roomIndex]->AddItem(itmPtr);
			DisplayInfo("You dropped '" + itmPtr->GetName() + "'.");
		} else {
			DisplayInfo("Can't find the item '" + usrInstruction.Goal + "' inside your inventory.");
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_EXIT) 
	{
		// User wants to exit game. Confirm and quit if so
		DisplayInfo("Are you sure you wish to quit? Type 'exit' again to confirm");
		Input::Instruction inst = Input::ReadUser();
		if (inst.Function == FUNCTION_EXIT)
			m_playingGame = false;
	}
}

int FindRoomIndex(std::string roomName)
{
	for (int i = 0; i < AllRooms.size(); i++)
	{
		std::string roomLower = Utils::ToLower(AllRooms[i]->Name);
		if (roomLower == roomName)
		{
			return i;
		}
	}
}

/*	Prints out room information in the format:
	> Room Name
	> Room Description
	> - - - - 
	> Items: Itm1, Itm2
	> Exits: Exit1, Exit2
	> NPCs: NPC1, NPC2
	*/
void PrintRoomInfo(Room& r)
{
	std::string out = "";
	std::string pre = "> ";

	std::string endl = "\n";
	out += pre + pre + r.Name + endl;
	out += pre + r.GetDescription() + endl;
	out += pre + "- - - - - - " + endl;
	
	// Append items if there are any
	int itmsSize = r.GetItemsSize();
	if (itmsSize > 0) {
		out += pre + "Items: ";
		for (unsigned int i = 0; i < itmsSize; i++) {
			out += r.GetItem(i)->GetName();
			if (i < itmsSize - 1) {
				out += ", ";
			}
		}
		out += endl;
	}
		
	// Append exists if exists
	int exitsSize = r.GetExitsSize();
	if (exitsSize > 0) {
		out += pre + "Exits: ";
		for (int i = 0; i < exitsSize; i++) {
			out += r.GetExit(i);
			if (i < exitsSize - 1)
				out += ", ";
		}
		out += endl;
	}

	// Append NPCs if exists
	int npcsSize= r.GetNPCSize();
	if (npcsSize > 0) {
		out += pre + "NPCs: ";
		for (int i = 0; i < npcsSize; i++) {
			out += r.GetNPC(i)->GetName();
			if (i < npcsSize - 1)
				out += ", ";
		}
		out += endl;
	}

	std::cout << out;
}

/*	Prints out the inventory of the user */
void PrintInventory()
{
	std::string msg = "";
	for (unsigned int i = 0; i < m_userInventory.GetSize(); i++) {
		std::string itemName = m_userInventory.GetItem(i)->GetName();
		if (itemName != "") {
			msg += itemName;
			if (i < m_userInventory.GetMaxSize() - 2)
				msg += ",";
		}
	}

	if (msg == "")
		msg = "You have no items in your inventory.";
	DisplayInfo(msg);
}

/* Disposes of all created objects ready for shutdown */
void DisposeGame()
{
	for (int i = 0; i < AllRooms.size(); i++) {
		if (AllRooms[i] != nullptr)
			delete AllRooms[i];
	}

	m_userInventory.Dispose();
}

/* Checks if all Spirits have their required item to proceed to ending */
bool SpiritsHaveItems()
{
	int hasItemCount = 0;
	Room* mainHall = AllRooms[0];
	int npcSize = mainHall->GetNPCSize();
	for (int i = 0; i < npcSize; i++) {
		NPC* npc = mainHall->GetNPC(i);
		if (npc->HasRequiredItem()) {
			hasItemCount++;
		}
	}

	return hasItemCount == npcSize;
}

void TryTakeItem(std::string itemName)
{
	bool canAdd = m_userInventory.CanAddItem();
	if (!canAdd) {
		DisplayInfo("You're inventory is looking a little full. Maybe you should make some space first before taking more things?");
		return;
	}

	bool contains = AllRooms[m_roomIndex]->ItemsContains(itemName);
	if (contains) {
		Item* itmPtr = AllRooms[m_roomIndex]->RemoveItem(itemName);

		bool hasAdded = m_userInventory.AddItem(itmPtr);
		if (hasAdded) {
			DisplayInfo("You pick up " + itmPtr->GetName());
		}
		else {
			// Shouldn't ever get to here
			// If you reach here, all hope is lost
			// glhf
			DisplayInfo("ERROR: Unable to add '" + itemName  + "' to your inventory");
		}
	}
	else {
		DisplayInfo("You tried to pick up '" + itemName + "' but couldn\'t find it");
	}
}