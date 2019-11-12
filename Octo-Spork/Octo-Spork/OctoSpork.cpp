#include <string>
#include <fstream>
#include <streambuf>

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

	DisplayInfo("Thanks for playing! Hope you enjoyed the journey");
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
		m_currentNPCPtr = m_allRooms[m_roomIndex]->GetNPC(usrInstruction.Goal);

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
		for (int i = 0; i < m_allRooms[m_roomIndex]->GetExitsSize(); i++)
		{
			std::string exitLower = Utils::ToLower(m_allRooms[m_roomIndex]->GetExit(i));
			std::string goalLower = Utils::ToLower(usrInstruction.Goal);
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
		PrintRoomInfo(*m_allRooms[m_roomIndex]);
	}
	else if (usrInstruction.Function == Function::FUNCTION_GIVE)
	{
		// User wants to give an item to the current NPC
		if (m_userInventory.Contains(usrInstruction.Goal)) {
			// Check if an NPC currently being spoken to
			if (m_currentNPCPtr == nullptr) {

				// Make user drop item since they're trying to GIVE before talking to NPC
				Item* itm = m_userInventory.RemoveItem(usrInstruction.Goal);
				m_allRooms[m_roomIndex]->AddItem(itm);

				DisplayInfo("You try to give '" + itm->GetName() + "'. However, you only hear a loud clunk as the item falls to the floor.");
				DisplayInfo("Maybe you should talk to someone first before trying to give an item?");
				InfoBuffer();

				PrintRoomInfo(*m_allRooms[m_roomIndex]);
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
		}
		else {
			DisplayInfo("You don't have '" + usrInstruction.Goal + "' in your inventory");
		}

		if (SpiritsHaveItems())
		{
			// ToDo: Implement Ending
			DisplayInfo("The three spirits suddenly vanish. The endless cloud wall seems to fade away, revealing a series of doors, all leading to the same path");

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

			PrintRoomInfo(*m_allRooms[m_roomIndex]);
			InfoBuffer();
		}
	}
	else if (usrInstruction.Function == Function::FUNCTION_TAKE)
	{
		// User picks up an item. 
		int size = m_allRooms[m_roomIndex]->GetItemsSize();
		if (size <= 0) {
			DisplayInfo("There are no items here.");
			return;
		}

		TryTakeItem(usrInstruction.Goal);
	}
	else if (usrInstruction.Function == Function::FUNCTION_DROP)
	{
		// User wants to drop an item
		bool contains = m_userInventory.Contains(usrInstruction.Goal);
		if (contains) {
			Item* itmPtr = m_userInventory.RemoveItem(usrInstruction.Goal);
			m_allRooms[m_roomIndex]->AddItem(itmPtr);
			DisplayInfo("You dropped '" + itmPtr->GetName() + "'.");
		}
		else {
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
	else if (usrInstruction.Function == Function::FUNCTION_EXAMINE)
	{
		Item* itmPtr = m_userInventory.TryGetItem(usrInstruction.Goal);
		if (itmPtr == nullptr) {
			DisplayInfo("You don't have '" + usrInstruction.Goal + "' in your inventory. Pick something up before examining");
		}
		else {
			DisplayInfo("You examine '" + itmPtr->GetName() + "' in your inventory...");
			DisplayInfo(itmPtr->GetName() + ": " + itmPtr->Examine());
		}
	}
}

void OctoSpork::PrintRoomInfo(Room& r)
{
	std::string out = "";
	std::string pre = "> ";

	std::string endl = "\n";
	out += pre + r.Name + endl;
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
	int npcsSize = r.GetNPCSize();
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
			DisplayInfo("You pick up " + itmPtr->GetName());
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
