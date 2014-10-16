#pragma once

#include <string>

class SynchEvent
{
public:
	SynchEvent(const std::string& id, float time);
	virtual ~SynchEvent();

	std::string GetId() const;
	float GetTime() const;

protected:
	std::string m_id;
	float m_time;
};
