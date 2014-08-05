#pragma once

#include "TrianglesRibbon.h"
#include <Math/Vec3.h>
#include <Math/Animation/AnimationCurve.h>
#include <vector>

class DecomposeToRibbon : public TrianglesRibbon
{
public:
	DecomposeToRibbon();
	~DecomposeToRibbon();

private:
	AnimationCurve<sm::Vec3>* CreateCurve(
		const sm::Vec3& basePosition,
		const sm::Vec3& normal,
		std::vector<sm::Vec3>& path,
		float startTime,
		float duration);

	AnimationCurve<float>* CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve);
};
