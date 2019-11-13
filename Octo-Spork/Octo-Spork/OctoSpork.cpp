#include <string>
#include <fstream>
#include <streambuf>

// Include RapidJson library files for deserializing Json
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

#include "OctoSpork.h"

OctoSpork::OctoSpork()
{
	int m_roomIndex = 0;
	bool m_playingGame = false;
	NPC* m_currentNPCPtr = nullptr;

	m_allRooms = std::vector<Room*>(0);
}

void OctoSpork::PlayGame()
{
	Init();

	Play();

	Dispose();
}

void OctoSpork::Init()
{
	// Initialize game by reading Json file with game data and populating arrays
	std::string data = ReadFile("resources/game.json");
	ParseJSON(data.c_str());

	m_roomIndex = 0;
}

/* Begin playing Octo-Spork, main entry */
void OctoSpork::Play()
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

	PrintRoomInfo(*m_allRooms[m_roomIndex]);
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

	InfoBuffer(3);
	DisplayInfo("Thanks for playing! Hope you enjoyed the journey");
	InfoBuffer(3);
}

/* Adds an amount of new line char's specified in 'count' */
void OctoSpork::InfoBuffer(int count)
{
	std::string el;
	for (int i = 0; i < count; i++) {
		el += "\n";
	}
	std::cout << el;
}

/* Displays a message to the console with a specific format */
inline void OctoSpork::DisplayInfo(std::string message)
{
	std::cout << "> " << message << std::endl;
}

/* Sleeps the main thread for the specified amount of milliseconds */
inline void OctoSpork::Sleep(int ms)
{
#if !_DEBUG
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

void OctoSpork::UpdateState(Input::Instruction usrInstruction)
{
	if (usrInstruction.Function == Function::FUNCTION_HELP) {
		Help(usrInstruction);
	}
	else if (usrInstruction.Function == Function::FUNCTION_TALK) {
		Talk(usrInstruction);
	}
	else if (usrInstruction.Function == Function::FUNCTION_ENTER) {
		Enter(usrInstruction);
	}
	else if (usrInstruction.Function == Function::FUNCTION_VIEW_INVENTORY) {
		// User viewing their inventory
		DisplayInfo("You look through your belongings...");
		PrintInventory();
	}
	else if (usrInstruction.Function == Function::FUNCTION_VIEW_ROOM) {
		// Print the room info to the user
		PrintRoomInfo(*m_allRooms[m_roomIndex]);
	}
	else if (usrInstruction.Function == Function::FUNCTION_GIVE) {
		Give(usrInstruction);
		
		// Check if Main Hall spirits have the required items to activate last 2 rooms
		if (SpiritsHaveItems()) {
			ActivateFinalRooms();
			
			InfoBuffer();
			DisplayInfo("The three spirits suddenly vanish. The endless cloud wall seems to fade away, revealing a series of doors, all leading to the same path");
			InfoBuffer();
			PrintRoomInfo(*m_allRooms[m_roomIndex]);
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_TAKE) {
		Take(usrInstruction);
	}
	else if (usrInstruction.Function == Function::FUNCTION_DROP) {
		Drop(usrInstruction);
	}
	else if (usrInstruction.Function == Function::FUNCTION_EXIT) {
		// User wants to exit game. Confirm and quit if so
		DisplayInfo("Are you sure you wish to quit? Type 'exit' again to confirm");
		Input::Instruction inst = Input::ReadUser();
		if (inst.Function == FUNCTION_EXIT)
			m_playingGame = false;
	}
	else if (usrInstruction.Function == Function::FUNCTION_EXAMINE) {
		Examine(usrInstruction);
	}
}

void OctoSpork::PrintRoomInfo(Room& r)
{
	std::string out = "";
	std::string pre = "> ";

	std::string endl = "\n";
	out += Utils::GetColor(DEFAULT_COLOR);
	out += pre + r.Name + endl;
	out += pre + r.GetDescription() + endl;
	out += pre + "- - - - - - " + endl;

	// Append items if there are any
	int itmsSize = r.GetItemsSize();
	if (itmsSize > 0) {
		out += pre + Utils::GetColor(ITEMS_COLOR) + "Items: ";
		for (unsigned int i = 0; i < itmsSize; i++) {
			out += r.GetItem(i)->GetName();
			if (i < itmsSize - 1) {
				out += ", ";
			}
		}
		out += endl;
		out += Utils::GetColor(DEFAULT_COLOR);
	}

	// Append exists if exists
	int exitsSize = r.GetExitsSize();
	if (exitsSize > 0) {
		out += pre + Utils::GetColor(EXITS_COLOR) + "Exits: ";
		for (int i = 0; i < exitsSize; i++) {
			out += r.GetExit(i);
			if (i < exitsSize - 1)
				out += ", ";
		}
		out += endl;
		out += Utils::GetColor(DEFAULT_COLOR);
	}

	// Append NPCs if exists
	int npcsSize = r.GetNPCSize();
	if (npcsSize > 0) {
		out += pre + Utils::GetColor(NPCS_COLOR) + "NPCs: ";
		for (int i = 0; i < npcsSize; i++) {
			out += r.GetNPC(i)->GetName();
			if (i < npcsSize - 1)
				out += ", ";
		}
		out += endl;
		out += Utils::GetColor(DEFAULT_COLOR);
	}
	
	out += Utils::GetColor(DEFAULT_COLOR);

	std::cout << out;
}

void OctoSpork::Dispose()
{
	for (int i = 0; i < m_allRooms.size(); i++) {
		if (m_allRooms[i] != nullptr)
			delete m_allRooms[i];
	}

	m_userInventory.Dispose();
}

void OctoSpork::PrintInventory()
{
	std::string msg = "";
	
	for (unsigned int i = 0; i < m_userInventory.GetSize(); i++) {
		msg += Utils::GetColor(ITEMS_COLOR);
		std::string itemName = m_userInventory.GetItem(i)->GetName();
		if (itemName != "") {
			msg += itemName;
			if (i < m_userInventory.GetMaxSize() - 1)
				msg += ",";
		}
	}

	if (msg == "") {
		msg += Utils::GetColor(DEFAULT_COLOR);
		msg = "You have no items in your inventory.";
	}

	msg += Utils::GetColor(DEFAULT_COLOR);
	DisplayInfo(msg);
}

bool OctoSpork::SpiritsHaveItems()
{
	int hasItemCount = 0;
	Room* mainHall = m_allRooms[0];
	int npcSize = mainHall->GetNPCSize();
	for (int i = 0; i < npcSize; i++) {
		NPC* npc = mainHall->GetNPC(i);
		if (npc->HasRequiredItem()) {
			hasItemCount++;
		}
	}

	return hasItemCount == npcSize;
}

int OctoSpork::FindRoomIndex(std::string roomName)
{
	for (int i = 0; i < m_allRooms.size(); i++) {
		std::string roomLower = Utils::ToLower(m_allRooms[i]->Name);
		if (roomLower == roomName) {
			return i;
		}
	}
	return -1;
}

std::string OctoSpork::ReadFile(std::string filename)
{
	std::ifstream t(filename);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	return str;
}

void OctoSpork::ParseJSON(const char* json)
{
	// Parse text input to RapidJson objects using the RapidJson library
	// All code inside this function comes from using the RapidJson library,
	// which is open-source and available online (https://github.com/Tencent/rapidjson)
	rapidjson::Document doc;
	doc.Parse(json);

	// Parse all Rooms and their info
	rapidjson::Value& rooms = doc["rooms"];
	if (rooms.IsArray()) {
		for ( rapidjson::Value::ConstValueIterator roomsItr = rooms.Begin(); roomsItr != rooms.End(); ++roomsItr ) {
			const rapidjson::Value& attribute = *roomsItr;
			if (!attribute.IsObject()) {
				continue;
			}

			// Parses a room object and add's it to m_allRooms
			Room* room = new Room();
			for (rapidjson::Value::ConstMemberIterator valItr = attribute.MemberBegin(); valItr != attribute.MemberEnd(); ++valItr) {
				if (valItr->name.IsString()) {
					// Get the key for the current Key/Value pair
					std::string key = valItr->name.GetString();
					// Check key against expected keys
					if (key == "name") {
						room->Name = valItr->value.GetString();
					}
					else if (key == "description") {
						room->SetDescription(valItr->value.GetString());
					}
					else if (key == "exits") {
						// Add all items from json to room
						for (auto& val : valItr->value.GetArray()) {
							room->AddExit(val.GetString());
						}
					}
					else if (key == "items") {
						for (auto& val : valItr->value.GetArray()) {
							// Create item for room
							auto itmName = val["name"].GetString();
							auto itmDesc = val["description"].GetString();
							
							Item* itm = new Item(itmName, itmDesc);
							room->AddItem(itm);
						}
					}
					else if (key == "npcs") {
						for (auto& val : valItr->value.GetArray()) {
							// Add NPC from it's config 
							NPCConfig conf = NPCConfig();
							conf.Name = val["name"].GetString();
							conf.RequiredItemName = val["required_item"].GetString();
							conf.Greeting = val["greeting"].GetString();
							conf.StandardResponse = val["standard_response"].GetString();
							conf.ExcessiveLimitCount = val["excessive_limit_count"].GetInt();
							conf.ExcessiveResponse = val["excessive_response"].GetString();
							conf.HasItemResponse = val["has_item_response"].GetString();
							conf.CompleteResponse = val["complete_response"].GetString();
							conf.IncorrectItemResponse = val["incorrect_item_response"].GetString();

							NPC* npc = new NPC(conf);
							// Add reward item if NPC Config contains one
							if (val.HasMember("reward_item")) {
								auto name = val["reward_item"]["name"].GetString();
								auto desc = val["reward_item"]["description"].GetString();

								Item* reward = new Item(name, desc);
								npc->SetReward(reward);
							}
							
							room->AddNPC(npc);
						}
					}
				}
			}
			m_allRooms.push_back(room);
		}
	}
}

void OctoSpork::Help(Input::Instruction instruction)
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

void OctoSpork::Talk(Input::Instruction instruction)
{
	// User talking to an NPC
	m_currentNPCPtr = m_allRooms[m_roomIndex]->GetNPC(instruction.Goal);

	if (m_currentNPCPtr == nullptr) {
		DisplayInfo("There is no one here named '" + instruction.Goal + "'.");
	}
	else {
		std::string speech = m_currentNPCPtr->GetSpeech();
		std::string line = Utils::GetColor(NPCS_COLOR) + m_currentNPCPtr->GetName() + Utils::GetColor(DEFAULT_COLOR) + ": " + speech;
		DisplayInfo(line);
	}
}

void OctoSpork::Enter(Input::Instruction instruction)
{
	// User enters new room
	for (int i = 0; i < m_allRooms[m_roomIndex]->GetExitsSize(); i++)
	{
		std::string exitLower = Utils::ToLower(m_allRooms[m_roomIndex]->GetExit(i));
		std::string goalLower = Utils::ToLower(instruction.Goal);
		if (exitLower == goalLower)
		{
			int newIndex = FindRoomIndex(goalLower);
			if (newIndex >= 0) {
				m_roomIndex = newIndex;
				break;
			}
		}
	}

	// Reset current talking to NPC since user changed room
	if (m_currentNPCPtr != nullptr)
		m_currentNPCPtr = nullptr;

	// Print information of new room
	PrintRoomInfo(*m_allRooms[m_roomIndex]);

	if (m_allRooms[m_roomIndex]->Name == "?") {
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
		DisplayInfo("Is that.." + Utils::GetColor(Colors::COLORS_RED) + "a helmet with pointy horns" + Utils::GetColor(DEFAULT_COLOR) + "?");

		Sleep(5000);
		DisplayInfo("It's" + Utils::GetColor(Colors::COLORS_RED) + " Odin" + Utils::GetColor(DEFAULT_COLOR) + "!");

		m_playingGame = false;
	}
}

void OctoSpork::Give(Input::Instruction instruction)
{
	// User wants to give an item to the current NPC
	if (m_userInventory.Contains(instruction.Goal)) {
		// Check if an NPC currently being spoken to
		if (m_currentNPCPtr == nullptr) {

			// Make user drop item since they're trying to GIVE before talking to NPC
			Item* itm = m_userInventory.RemoveItem(instruction.Goal);
			m_allRooms[m_roomIndex]->AddItem(itm);

			DisplayInfo("You try to give '" + itm->GetName() + "'. However, you only hear a loud clunk as the item falls to the floor.");
			DisplayInfo("Maybe you should talk to someone first before trying to give an item?");
			InfoBuffer();

			PrintRoomInfo(*m_allRooms[m_roomIndex]);
			return;
		}

		bool completedNPC = m_currentNPCPtr->GiveItem(instruction.Goal);
		std::string speech = m_currentNPCPtr->GetSpeech();
		std::string line = Utils::GetColor(NPCS_COLOR) + m_currentNPCPtr->GetName() + Utils::GetColor(DEFAULT_COLOR) + ": " + speech;
		DisplayInfo(line);

		if (completedNPC) {
			// Remove item from inventory, only if successful
			Item* itm = m_userInventory.RemoveItem(instruction.Goal);
			delete itm;

			Item* rewardItm = m_currentNPCPtr->PopReward();
			if (rewardItm != nullptr) {
				// Buffer between speech of NPC and reward
				InfoBuffer();
				DisplayInfo("'" + Utils::GetColor(NPCS_COLOR) + m_currentNPCPtr->GetName() + Utils::GetColor(DEFAULT_COLOR) + "' rewards you with '" + Utils::GetColor(ITEMS_COLOR) + rewardItm->GetName() + Utils::GetColor(DEFAULT_COLOR) + "'");

				m_userInventory.AddItem(rewardItm, true);
			}
		}
	}
	else {
		DisplayInfo("You don't have '" + instruction.Goal + "' in your inventory");
	}
}

void OctoSpork::Take(Input::Instruction instruction)
{
	// User picks up an item. 
	int size = m_allRooms[m_roomIndex]->GetItemsSize();
	if (size <= 0) {
		DisplayInfo("There are no items here.");
		return;
	}

	TryTakeItem(instruction.Goal);
}

void OctoSpork::TryTakeItem(std::string itemName)
{
	bool canAdd = m_userInventory.CanAddItem();
	if (!canAdd) {
		DisplayInfo("You're inventory is looking a little full. Maybe you should make some space first before taking more things?");
		return;
	}

	bool contains = m_allRooms[m_roomIndex]->ItemsContains(itemName);
	if (contains) {
		Item* itmPtr = m_allRooms[m_roomIndex]->RemoveItem(itemName);

		bool hasAdded = m_userInventory.AddItem(itmPtr);
		if (hasAdded) {
			DisplayInfo("You pick up " + Utils::GetColor(ITEMS_COLOR) + itmPtr->GetName() + Utils::GetColor(DEFAULT_COLOR));
		}
		else {
			// Shouldn't ever get to here
			// If you reach here, all hope is lost
			// glhf
			DisplayInfo("ERROR: Unable to add '" + itemName + "' to your inventory");
		}
	}
	else {
		DisplayInfo("You tried to pick up '" + itemName + "' but couldn\'t find it");
	}
}

void OctoSpork::Drop(Input::Instruction instruction)
{
	// User wants to drop an item
	bool contains = m_userInventory.Contains(instruction.Goal);
	if (contains) {
		Item* itmPtr = m_userInventory.RemoveItem(instruction.Goal);
		m_allRooms[m_roomIndex]->AddItem(itmPtr);
		DisplayInfo("You dropped '" + Utils::GetColor(ITEMS_COLOR) + itmPtr->GetName() + Utils::GetColor(DEFAULT_COLOR) + "'.");
	}
	else {
		DisplayInfo("Can't find the item '" + instruction.Goal + "' inside your inventory.");
	}
}

void OctoSpork::Examine(Input::Instruction instruction)
{
	Item* itmPtr = m_userInventory.TryGetItem(instruction.Goal);
	if (itmPtr == nullptr) {
		DisplayInfo("You don't have '" + instruction.Goal + "' in your inventory. Pick something up before examining");
	}
	else {
		DisplayInfo("You examine '" + Utils::GetColor(ITEMS_COLOR) + itmPtr->GetName() + Utils::GetColor(DEFAULT_COLOR) + "' in your inventory...");
		DisplayInfo(Utils::GetColor(ITEMS_COLOR) + itmPtr->GetName() + Utils::GetColor(DEFAULT_COLOR) + ": " + itmPtr->Examine());
	}
}


void OctoSpork::ActivateFinalRooms()
{
	// Create room after cloud wall
	std::vector<std::string> exits = { "Main Hall", "?" };
	Room* finalRoom = new Room("Realm Beyond Realms", "The cloud wall revealed this room. This must lead to a way out! Surely...", exits);
	Item* itm = new Item("Dust Pile", "*Acho*");
	finalRoom->AddItem(itm);
	m_allRooms.push_back(finalRoom);

	// Add new room as exit in Main Hall
	m_allRooms[0]->AddExit(finalRoom->Name);

	// Create room after Realm Beyond Realms, "?"
	exits = { };
	Room* unknown = new Room("?", "There's just... nothing.", exits);
	m_allRooms.push_back(unknown);
}