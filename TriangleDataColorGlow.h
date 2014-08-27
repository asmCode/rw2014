#pragma once

#include <Math/Vec3.h>
#include <Math/Vec4.h>
#include <Math/Matrix.h>

class TriangleDataColorGlow
{
public:
	sm::Vec4 Color;
	sm::Matrix Transform;
	float GlowPower;
};
