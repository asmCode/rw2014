#pragma once

#include "TrianglesRibbon.h"
#include <Math/Vec3.h>
#include <Math/Animation/AnimationCurve.h>
#include <vector>

class ComposeFromRibbon : public TrianglesRibbon
{
public:
	ComposeFromRibbon();
	~ComposeFromRibbon();

	void Update(float time, float deltaTime);

	void ProcessTriangle(float time, int i);

private:
	AnimationCurve<sm::Vec3>* CreateCurve(
		const sm::Vec3& basePosition,
		const sm::Vec3& normal,
		SceneElement::Path* path,
		int endKeyIndex,
		float spread,
		float maxDelay);

	AnimationCurve<float>* CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale);
};
