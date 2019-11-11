#include "OctoSpork.h"

OctoSpork::OctoSpork()
{
	std::vector<Room*> AllRooms;
	Inventory m_userInventory;
	int m_roomIndex = 0;
	bool m_playingGame = false;
	NPC* m_currentNPCPtr = nullptr;
}

void OctoSpork::PlayGame()
{
	Init();

	Play();

	Dispose();
}

void OctoSpork::Init()
{
	std::vector<std::string> exits;

	// Spawn Room, Main Hall
	exits = { "North Hallway", "Staircase" };
	Room* mainHall = new Room("Main Hall", "Where I woke up. There's a massive sign on the wall with it's name", exits);

#if _DEBUG
	// Here's your cheats, ya filthy animal
	mainHall->AddItem(new Item(LIGHTNING_STONE, LIGHTNING_STONE_DESCRIPTION));
	mainHall->AddItem(new Item(EARTH_STONE, EARTH_STONE_DESCRIPTION));
	mainHall->AddItem(new Item(LIGHT_STONE, LIGHT_STONE_DESCRIPTION));
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
	Room* nHallway = new Room("North Hallway", "A long hallway that leads down one side of the facility", exits);
	nHallway->AddItem(new Item("Glass Shard", "I should be careful with this. Don't wanna do more harm"));
	nHallway->AddItem(new Item("Luden Statue", "Aww, isn't he a cute lil' guy"));

	exits = { "North Hallway", "MI035" };
	Room* eHallway = new Room("East Hallway", "Hallway that takes me to the back of the complex. Seems very dark down here", exits);
	eHallway->AddItem(new Item("Blank Paper", "A blank piece of paper, could be useful with a pen"));

	// Ground Floor - Rooms
	// MI034
	exits = { "North Hallway", "MI035" };
	Room* mi034 = new Room("MI034", "A quiet little room near the side of the facility with a ghost. Wonder how he got here...", exits);
	mi034->AddItem(new Item("Computer Mouse", "A computer mouse, used for controlling a PC"));
	mi034->AddItem(new Item("Broken Headset", "Just a broken headset. Nothing special here"));

	NPCConfig govrConfig = NPCConfig();
	govrConfig.Name = "Ghost of VR";
	govrConfig.RequiredItemName = "Vive Controller";
	govrConfig.Greeting = "Greetings wanderer. What brings you along my path? Is it the promise of loot or to free me from this cursed room?";
	govrConfig.StandardResponse = "Please, wanderer. I have been here for 500 years now. My death is the only thing that bounds me to the realm. Can you help free me?";
	govrConfig.ExcessiveResponse = "Do you wish to torture me by returning so often? Help me or leave me be.";
	govrConfig.HasItemResponse = "Ah, finally! Now I may rest in peace. I am eternally greateful. Thank you. And here, take this I found lying around";
	govrConfig.CompleteResponse = "You have served me well. Now go, let me play VR.";
	govrConfig.IncorrectItemResponse = "What is this? Don't waste my time with irrelevent things.";
	govrConfig.ExcessiveLimitCount = 15;

	NPC* ghostVR = new NPC(govrConfig);
	Item* stone = new Item(LIGHT_STONE, LIGHT_STONE_DESCRIPTION);
	ghostVR->SetReward(stone);

	mi034->AddNPC(ghostVR);

	// MI035
	exits = { "East Hallway", "MI034" };
	Room* mi035 = new Room("MI035", "There's hardly any light in here but I can just about make out a couple things.", exits);
	mi035->AddItem(new Item("Ominous Candle", "Not sure how a candle can be ominous but, here we are."));
	mi035->AddItem(new Item("Vive Controller", "A VR controller. Could be useful to someone who has one missing"));

	// Floor 1 to 2 Staircase 
	exits = { "Main Hall", "F1 Landing" };
	Room* staircase = new Room("Staircase", "A crumbling staircase that leads upstairs. It might contain more rooms to check out", exits);
	staircase->AddItem(new Item("Stone", "It's a rock, nothing special"));
	staircase->AddItem(new Item("Small rock","It's an even smaller rock. Even less special"));

	// Floor 2
	exits = { "Staircase", "F1 North Hallway", "F1 West Hallway" };
	Room* f1Landing = new Room("F1 Landing", "A massive open space, full of old and rusted parts. From here I can see three room, all look abandoned and filled with broken glass", exits);
	f1Landing->AddItem(new Item("Electrical Parts", "So many broken parts. Maybe these could be useful?"));
	f1Landing->AddItem(new Item("Lantern", "Good for lighting the way."));
	f1Landing->AddItem(new Item("Empty Water Bottle", "A water bottle, but nothing is inside."));

	exits = { "F1 East Hallway", "F1 Landing", "MI102c", "F1 West Hallway" };
	Room* f1nHallway = new Room("F1 North Hallway", "Another hallway that leads to more dark places", exits);
	f1nHallway->AddItem(new Item("Splint", "Useless"));

	exits = { "F1 North Hallway", "F1 East Hallway", "MI102c" };
	Room* f1eHallway = new Room("F1 East Hallway", "An almost enclosed space with complete darkness at the other end. However, there seems to be a head down there...", exits);
	f1eHallway->AddItem(new Item("Shard of the Unknown", "A very creepy shard"));
	f1eHallway->AddItem(new Item("Empty Mug", "Just an big empty mug with a logo on the side saying 'Sports Direct'"));
	f1eHallway->AddItem(new Item("£5 Note", "A note. Seems like it's of a foreign currency. Never seen one of these before"));

	NPCConfig headConfig = NPCConfig();
	headConfig.Name = "Abandoned Head";
	headConfig.RequiredItemName = "Lantern";
	headConfig.Greeting = "Ahh, nice to meet ya, fellow prisoner. NIce to see another prisoner come through. Trut me, I've been stuck here for a while now and seen plenty o' fellows. Maybe you can help me?";
	headConfig.StandardResponse = "If I were you, I'd see about convincing them spirits to let you out. I have a stone which might help you! Just find me a torch or something...";
	headConfig.ExcessiveLimitCount = 5;
	headConfig.HasItemResponse = "Ah, thank you! In return, take this. I thought it could light up the path but to no avail.";
	headConfig.ExcessiveResponse = "I'm just a head. Stop asking me questions! You might have a body still but are you sure you have a brain?";
	headConfig.IncorrectItemResponse = "What am I gonna do with this. I can barely see it!";

	NPC* headNPC = new NPC(headConfig);
	headNPC->SetReward(new Item(LIGHTNING_STONE, LIGHTNING_STONE_DESCRIPTION));
	f1eHallway->AddNPC(headNPC);

	exits = { "F1 Landing", "F1 North Hallway", "MI102a", "MI102b" };
	Room* f1wHallway = new Room("F1 West Hallway", "Another hallway. A wall of rolling cloud fills the end of the hallway. Seems impassable", exits);

	// Floor 2 - Rooms
	exits = { "F1 Landing" };
	Room* mi102a = new Room("MI102a", "A very open room, again filled with loads of broken electrical parts", exits);
	Item* earthStone = new Item(EARTH_STONE, EARTH_STONE_DESCRIPTION);
	mi102a->AddItem(earthStone);

	Room* mi102b = new Room("MI102b", "You only just manage to get into the room. There", exits);

	exits = { "F1 North Hallway" };
	Room* mi102c = new Room("MI102c", "Another room populated with broken electrical parts. These parts must have been using by the facility before", exits);

	// Add all rooms to Vector
	m_allRooms.push_back(mainHall);
	m_allRooms.push_back(nHallway);
	m_allRooms.push_back(eHallway);
	m_allRooms.push_back(mi034);
	m_allRooms.push_back(mi035);
	m_allRooms.push_back(staircase);
	m_allRooms.push_back(f1Landing);
	m_allRooms.push_back(f1nHallway);
	m_allRooms.push_back(f1eHallway);
	m_allRooms.push_back(f1wHallway);
	m_allRooms.push_back(mi102a);
	m_allRooms.push_back(mi102b);
	m_allRooms.push_back(mi102c);
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