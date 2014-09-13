#pragma once

#include "IRibbonCurveBuilder.h"

class RibbonCurveDestination : public IRibbonCurveBuilder
{
public:
	AnimationCurve<sm::Vec3>* CreateCurve(
		const sm::Vec3& basePosition,
		const sm::Vec3& normal,
		SceneElement::Path* path,
		int endKeyIndex,
		float spread,
		float maxDelay);

	AnimationCurve<float>* CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale);
};
