#pragma once

class TimeControl;

class Stopwatch
{
public:
	Stopwatch(bool start);
	~Stopwatch();

	void Start();
	void ResetAndStart();
	float GetTime() const;

private:
	TimeControl* m_timeControl;
};
