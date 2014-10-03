#include "Stopwatch.h"
#include "../GraphicsLibrary/TimeControl.h"

Stopwatch::Stopwatch(bool start = false)
{
	m_timeControl = new TimeControl();

	if (start)
		m_timeControl->Start();
}

Stopwatch::~Stopwatch()
{
	delete m_timeControl;
}

void Stopwatch::Start()
{
	m_timeControl->Start();
}

void Stopwatch::ResetAndStart()
{
	m_timeControl->Start();
}

float Stopwatch::GetTime() const
{
	return m_timeControl->GetTime();
}
