#include "LinearCurve.h"
#include "../MathUtils.h"

float LinearCurve::Evaluate(float begin, float end, float time)
{
	time = MathUtils::Clamp(time, 0.0f, 1.0f);

	float change = end - begin;

	return change * time + begin;
};
