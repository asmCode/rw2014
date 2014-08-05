#pragma once

#include <Math/Vec3.h>
#include <Graphics/Interpolators/IInterpolator.h>
#include <Math/Animation/AnimationCurve.h>

class DebugUtils
{
public:
	static void DrawCurve(IInterpolator<sm::Vec3>* curve, float timeStep, const sm::Vec3& color);
	static void DrawCurve(AnimationCurve<sm::Vec3>& curve, float timeStep, const sm::Vec3& color);
};
