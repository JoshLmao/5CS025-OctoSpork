#pragma once

#include <string>
#include <iostream>

#include "Item.h"

struct NPCConfig {
	std::string Name;
	std::string RequiredItemName;
	
	/* First response to player when greeted for first time */
	std::string Greeting;
	/* Standard response to player from the NPC */
	std::string StandardResponse;
	/* Response to the player when they've annoyed the NPC */
	std::string ExcessiveResponse;
	/* Response to player when NPC recieves the right item */
	std::string HasItemResponse;
	/* Response to player when NPC gets given the wrong item */
	std::string IncorrectItemResponse;
	/* Response to player after NPC has reciever their item and no longer needs anything */
	std::string CompleteResponse;

	/* Amount of times of interact before NPC gets annoyed */
	int ExcessiveLimitCount;
};

class NPC {
public:
	NPC();
	NPC(NPCConfig config);

	/* Get the current speech from the NPC to say */
	std::string GetSpeech();
	/* Give an item to the NPC */
	bool GiveItem(std::string itemName);

	/* Is the NPC annoyed with the player */
	bool IsAnnoyed();
	/* Get the name of the NPC */
	std::string GetName();
	/* Does the NPC have the item they require? */
	bool HasRequiredItem();

	/* Sets the item reward for this NPC */
	void SetReward(Item* itm);
	/*	Get the reward that this NPC gives to the player once the player has given them the right item
		NPC forgets about the pointer after this is called
		*/
	Item* PopReward();

private:
	NPCConfig m_config;

	/* Name of item attempted to be given to the NPC */
	std::string m_attemptedItem;
	/* The item to reward the player with */
	Item* m_reward;

	/* Has the NPC greeted the player before */
	bool m_hasGreeted;
	/* Does the NPC has the item it needs from the Player */
	bool m_hasRequiredItem;
	/* Current amount of times NPC has been interacted with */
	int m_interactCount;
};