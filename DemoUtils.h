#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <vector>

class DemoUtils
{
public:
	static sm::Vec3 GetRandomVector();
	static float GetPathLength(std::vector<sm::Vec3>& path);
	static sm::Matrix GetTransform(sm::Vec3 triangle[3]);
};
