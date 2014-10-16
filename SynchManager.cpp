#include "SynchManager.h"
#include "SynchEvent.h"

SynchManager::SynchManager()
{
}

SynchManager::~SynchManager()
{
}

void SynchManager::Addevent(SynchEvent* synchEvent)
{
	m_events.push_back(synchEvent);
}

void SynchManager::Update(float time)
{
}

SynchEvent* SynchManager::GetAndRemoveEvent()
{
	return NULL;
}

void SynchManager::SortEventsByTime()
{
}
