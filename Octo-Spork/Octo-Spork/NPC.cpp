#include "NPC.h"
#include "Utils.h"

NPC::NPC()
{
	m_config = NPCConfig();
	m_config.Name = "";
	m_config.RequiredItemName = "";

	m_config.Greeting = "Hello traveller";
	m_config.StandardResponse = "Welcome back";
	m_config.HasItemResponse = "Thank you";
	m_config.IncorrectItemResponse = "Wrong item";
	m_config.ExcessiveResponse = "You're annoying. Go away";
	m_config.CompleteResponse = "I have nothing more to offer you";

	m_config.ExcessiveLimitCount = 25;

	m_hasGreeted = false;
	m_hasRequiredItem = false;
	m_interactCount = 0;
	m_attemptedItem = "";
	m_reward = nullptr;
}

NPC::NPC(NPCConfig config)
{
	m_config = config;

	m_hasGreeted = false;
	m_hasRequiredItem = false;
	m_interactCount = 0;
	m_attemptedItem = "";
	m_reward = nullptr;
}

std::string NPC::GetSpeech()
{
	m_interactCount++;

	// First time user has met NPC
	if (!m_hasGreeted) {
		m_hasGreeted = true;
		return m_config.Greeting;
	}

	// User tried giving NPC an item
	if (m_attemptedItem != "") {
		m_attemptedItem = "";
		if (m_hasRequiredItem) {
			return m_config.HasItemResponse;
		} else {
			return m_config.IncorrectItemResponse;
		}
	}
	
	// Is NPC is annoyed with Player
	if (IsAnnoyed()) {
		return m_config.ExcessiveResponse;
	}

	// If NPC already has item and player tries to talk again
	if (m_hasRequiredItem) {
		return m_config.CompleteResponse;
	}

	return m_config.StandardResponse;
}

bool NPC::GiveItem(std::string itemName)
{
	if (m_hasRequiredItem)
		return false;

	m_attemptedItem = itemName;
	if (Utils::ToLowerCompare(m_attemptedItem, m_config.RequiredItemName)) {
		m_hasRequiredItem = true;
		return true;
	}
	return false;
}

bool NPC::IsAnnoyed()
{
	return m_interactCount >= m_config.ExcessiveLimitCount;
}

std::string NPC::GetName()
{
	return m_config.Name;
}

bool NPC::HasRequiredItem()
{
	return m_hasRequiredItem;
}

void NPC::SetReward(Item* itm)
{
	m_reward = itm;
}

Item* NPC::PopReward()
{
	Item* ptr = m_reward;
	m_reward = nullptr;
	return ptr;
}