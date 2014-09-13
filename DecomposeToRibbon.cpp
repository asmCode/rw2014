#include "DecomposeToRibbon.h"
#include "TriangledMesh.h"
#include "Triangle.h"
#include "DemoUtils.h"
#include "DebugUtils.h"
#include "SceneElement/Path.h"
#include "SceneElement/Key.h"
#include <Math/Animation/QuarticOut.h>
#include <Utils/Randomizer.h>
#include <Graphics/Interpolators/TCBInterpolator.h>
#include <Math/Animation/AnimationCurve.h>

DecomposeToRibbon::DecomposeToRibbon()
{
}

DecomposeToRibbon::~DecomposeToRibbon()
{
}

AnimationCurve<sm::Vec3>* DecomposeToRibbon::CreateCurve(
	const sm::Vec3& basePosition,
	const sm::Vec3& normal,
	SceneElement::Path* path,
	int endKeyIndex,
	float spread,
	float maxDelay)
{	
	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

	static Randomizer random;

	const std::vector<SceneElement::Key*>& keys = path->Keys;

	float endTime = path->Keys[endKeyIndex - 1]->Time;
	float timeScale = random.GetFloat(1.0f, (endTime + maxDelay) / endTime);

	float firstMoveDistance = random.GetFloat(1.0f, 4.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	curve->AddKeyframe(keys[0]->Time * timeScale, basePosition);
	curve->AddKeyframe(keys[1]->Time * timeScale, firstMovePosition);

	for (uint32_t i = 2; i < endKeyIndex; i++)
	{
		curve->AddKeyframe(keys[i]->Time * timeScale, path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
	}

	curve->SmoothTangents();

	return curve;
}

AnimationCurve<float>* DecomposeToRibbon::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float startTime = transformCurve->GetStartTime();
	float endTime = transformCurve->GetEndTime();

	curve->AddKeyframe(startTime, 1.0f);
	curve->AddKeyframe(startTime + 3.0f, minScale);
	curve->AddKeyframe((endTime + startTime) / 2.0f, minScale);
	curve->AddKeyframe(endTime - 3.0f, minScale);
	curve->AddKeyframe(endTime, 0.0f);
	//curve->SmoothTangents();

	return curve;
}
