#pragma once

#include <vector>

class SynchEvent;

class SynchManager
{
public:
	SynchManager();
	virtual ~SynchManager();

	void Addevent(SynchEvent* synchEvent);
	void Update(float time);
	SynchEvent* GetAndRemoveEvent();

	void SortEventsByTime();

protected:
	std::vector<SynchEvent*> m_events;
	float m_time;
};
