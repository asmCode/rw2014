#pragma once

#include "IAnimationCurve.h"
#include "../MathUtils.h"
#include <math.h>

template <typename T>
class QuadOut : public IAnimationCurve<T>
{
public:
	T Evaluate(const T& begin, const T& end, float time)
	{
		time = MathUtils::Clamp(time, 0.0f, 1.0f);

		T c = end - begin;

		return -c * time * (time - 2) + begin;
	};
};
