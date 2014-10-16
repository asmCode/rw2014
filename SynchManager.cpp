#include "SynchManager.h"
#include "SynchEvent.h"
#include "SynchEventSort.h"
#include <algorithm>

SynchManager::SynchManager() :
	m_time(0.0f)
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
	m_time = time;
}

SynchEvent* SynchManager::GetAndRemoveEvent()
{
	if (m_events.size() == 0)
		return NULL;

	int lastElementIndex = m_events.size() - 1;

	if (m_time >= m_events[lastElementIndex]->GetTime())
	{
		SynchEvent* synchEvent = m_events[lastElementIndex];
		m_events.pop_back();

		return synchEvent;
	}

	return NULL;
}

void SynchManager::SortEventsByTime()
{
	static SynchEventSort synchEventsort; // descending!! (it is much easier to pop event from the end of the vector).
	std::sort(m_events.begin(), m_events.end(), synchEventsort);
}
