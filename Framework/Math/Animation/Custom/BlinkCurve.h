#pragma once

#include "../IAnimationCurve.h"
#include "../../MathUtils.h"
#include <math.h>

template <typename T, typename TUp, typename TDown>
class BlinkCurve : public IAnimationCurve<T>
{
public:
	BlinkCurve(float breakTime) :
		m_breakTime(breakTime)
	{
	}

	T Evaluate(const T& begin, const T& end, float time)
	{
		time = MathUtils::Clamp(time, 0.0f, 1.0f);

		if (time <= m_breakTime)
			return m_up.Evaluate(begin, end, time / m_breakTime);
		else
			return m_down.Evaluate(end, begin, (time - m_breakTime) / (1.0f - m_breakTime));
	};

private:
	float m_breakTime;
	TUp m_up;
	TDown m_down;
};
