#include "NPC.h"

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
}

NPC::NPC(NPCConfig config)
{
	m_config = config;

	m_hasGreeted = false;
	m_hasRequiredItem = false;
	m_interactCount = 0;
}

std::string NPC::GetSpeech()
{
	m_interactCount++;

	if (m_interactCount >= m_config.ExcessiveLimitCount)
	{
		return m_config.ExcessiveResponse;
	}

	// First time user has met NPC
	if (!m_hasGreeted) {
		m_hasGreeted = 1;
		return m_config.Greeting;
	}

	// User given item to NPC.
	if (m_hasRequiredItem) {
		return m_config.HasItemResponse;
	}

	return m_config.StandardResponse;
}

bool NPC::GiveItem(std::string itemName)
{
	if (IsAnnoyed())
		return false;

	if (itemName == m_config.RequiredItemName || m_hasRequiredItem) {
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