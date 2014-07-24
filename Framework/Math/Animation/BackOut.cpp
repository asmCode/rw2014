#include "BackOut.h"
#include "../MathUtils.h"

float BackOut::Evaluate(float begin, float end, float time)
{
	time = MathUtils::Clamp(time, 0.0f, 1.0f);

	float c = end - begin;

	float s = 1.70158f;
	time -= 1.0f;
 	return c * (time * time * ((s + 1) * time + s) + 1) + begin;
};
