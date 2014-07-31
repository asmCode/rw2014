#pragma once

#include <Math/Vec3.h>
#include <vector>

class DemoUtils
{
public:
	static sm::Vec3 GetRandomVector();
	static float GetPathLength(std::vector<sm::Vec3>& path);
};
