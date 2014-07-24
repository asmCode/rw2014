#pragma once

template <typename T>
class IAnimationCurve
{
public:
	virtual ~IAnimationCurve() {};

	virtual T Evaluate(const T& begin, const T& end, float time) = 0;
};
