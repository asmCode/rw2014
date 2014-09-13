#pragma once

#include <Math/Vec3.h>
#include <Math/Animation/AnimationCurve.h>
#include "../SceneElement/Path.h"
#include <vector>

class IRibbonCurveBuilder
{
public:
	virtual ~IRibbonCurveBuilder() {};

	virtual AnimationCurve<sm::Vec3>* CreateCurve(
		const sm::Vec3& basePosition,
		const sm::Vec3& normal,
		SceneElement::Path* path,
		int endKeyIndex,
		float spread,
		float maxDelay) = 0;

	virtual AnimationCurve<float>* CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale) = 0;
};
