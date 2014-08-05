#include "DemoUtils.h"
#include <Utils/Randomizer.h>
#include <stdint.h>

sm::Vec3 DemoUtils::GetRandomVector()
{
	static Randomizer random;

	return sm::Vec3(
		random.GetFloat(-1, 1),
		random.GetFloat(-1, 1),
		random.GetFloat(-1, 1)).GetNormalized();
}

float DemoUtils::GetPathLength(std::vector<sm::Vec3>& path)
{
	float length = 0.0f;

	for (uint32_t i = 0; i < path.size() - 1; i++)
		length += (path[i] - path[i + 1]).GetLength();

	return length;
}
