#pragma once

#include "IAnimationCurve.h"

template <typename T>
class LinearCurve : public IAnimationCurve<T>
{
public:
	float Evaluate(const T& begin, const T& end, float time)
	{
		time = MathUtils::Clamp(time, 0.0f, 1.0f);

		T change = end - begin;

		return change * time + begin;
	}
};
