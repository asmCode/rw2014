#pragma once

#include "IAnimationCurve.h"
#include "../MathUtils.h"

template <typename T>
class QuarticOut : public IAnimationCurve<T>
{
public:
	T Evaluate(const T& begin, const T& end, float time)
	{
		time = MathUtils::Clamp(time, 0.0f, 1.0f);

		T c = end - begin;
		time--;
		return ((-c) * (time*time*time*time - 1)) + begin;
	};

};
