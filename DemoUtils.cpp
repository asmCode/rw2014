#include "DemoUtils.h"
#include <Utils/Randomizer.h>

sm::Vec3 DemoUtils::GetRandomVector()
{
	static Randomizer random;

	return sm::Vec3(
		random.GetFloat(-1, 1),
		random.GetFloat(-1, 1),
		random.GetFloat(-1, 1)).GetNormalized();
}

