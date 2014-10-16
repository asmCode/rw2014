#pragma once

class SynchEvent;

// descending
class SynchEventSort
{
public:
	bool operator() (const SynchEvent* event1, const SynchEvent* event2);
};
