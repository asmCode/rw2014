#pragma once

class TimeControl;

class Stopwatch
{
public:
	Stopwatch(bool start);
	~Stopwatch();

	void Start();
	float GetTime() const;

private:
	TimeControl* m_timeControl;
};
