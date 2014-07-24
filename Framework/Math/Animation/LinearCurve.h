#pragma once

#include "IAnimationCurve.h"

class LinearCurve : public IAnimationCurve<float>
{
public:
	float Evaluate(float begin, float end, float time);
};
