#include "SynchEvent.h"

SynchEvent::SynchEvent(const std::string& id, float time) :
	m_id(id),
	m_time(time)
{
}

SynchEvent::~SynchEvent()
{
}

std::string SynchEvent::GetId() const
{
	return m_id;
}

float SynchEvent::GetTime() const
{
	return m_time;
}
