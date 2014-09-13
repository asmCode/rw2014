#pragma once

#include <Math/Vec3.h>
#include <Math/Vec4.h>
#include <Math/Matrix.h>
#include <vector>

class ManCam;

class DemoUtils
{
public:
	static sm::Vec3 GetRandomVector();
	static float GetPathLength(std::vector<sm::Vec3>& path);
	static sm::Matrix GetTriangleTransform(sm::Vec3* triangle);
	static std::string ToText(const sm::Vec3& value, const std::string& separator = ";");
	static std::string ToText(const sm::Vec4& value, const std::string& separator = ";");
	static sm::Vec3 ParseVector3(const std::string& vectorStr, const std::string& separator = ";");
	static sm::Vec4 ParseVector4(const std::string& vectorStr, const std::string& separator = ";");
	static void SaveCamera(ManCam* camera, int slot);
	static void LoadCamera(ManCam* camera, int slot);
};
