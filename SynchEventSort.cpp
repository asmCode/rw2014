#include "SynchEventSort.h"
#include "SynchEvent.h"

bool SynchEventSort::operator() (const SynchEvent* event1, const SynchEvent* event2)
{
	return event1->GetTime() > event2->GetTime();
}
