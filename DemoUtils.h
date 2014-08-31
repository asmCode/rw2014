#pragma once

#include <Math/Vec3.h>
#include <Math/Vec4.h>
#include <Math/Matrix.h>
#include <vector>

class DemoUtils
{
public:
	static sm::Vec3 GetRandomVector();
	static float GetPathLength(std::vector<sm::Vec3>& path);
	static sm::Matrix GetTriangleTransform(sm::Vec3* triangle);
	static sm::Vec3 ParseVector3(const std::string& vectorStr, const std::string& separator = ";");
	static sm::Vec4 ParseVector4(const std::string& vectorStr, const std::string& separator = ";");
};
