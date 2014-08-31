#include "DemoUtils.h"
#include <Utils/Randomizer.h>
#include <Utils/StringUtils.h>
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

sm::Matrix DemoUtils::GetTriangleTransform(sm::Vec3* triangle)
{
	sm::Vec3 center = (triangle[0] + triangle[1] + triangle[2]) / 3.0f;

	sm::Vec3 normal = -((triangle[1] - triangle[0]) * (triangle[2] - triangle[0])).GetNormalized();
	sm::Vec3 triangleBase = triangle[1] - triangle[0];

	sm::Vec3 up = (normal * triangleBase).GetNormalized();

	sm::Matrix rotate;
	if (normal != sm::Vec3(0, 0, 0) &&
		up != sm::Vec3(0, 0, 0))
		rotate = sm::Matrix::CreateLookAt(normal, up);
	else
		rotate = sm::Matrix::IdentityMatrix();

	return
		sm::Matrix::TranslateMatrix(center) *
		rotate;
}

sm::Vec3 DemoUtils::ParseVector3(const std::string& vectorStr, const std::string& separator)
{
	sm::Vec3 vec;
	sscanf(vectorStr.c_str(), "%f;%f;%f", &vec.x, &vec.y, &vec.z);
	return vec;
}

sm::Vec4 DemoUtils::ParseVector4(const std::string& vectorStr, const std::string& separator)
{
	sm::Vec4 vec;
	sscanf(vectorStr.c_str(), "%f;%f;%f;%f", &vec.x, &vec.y, &vec.z, &vec.w);
	return vec;
}
