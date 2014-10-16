#pragma once

#include <string>

class SynchManager;

class MidiSynchEventsLoader
{
public:
	static void LoadFromFile(const std::string& path, SynchManager* synchManager);
};
