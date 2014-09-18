#include "RibbonCurveDestination.h"
#include "../SceneElement/Key.h"
#include "../DemoUtils.h"
#include <Utils/Randomizer.h>
#include <Math/Animation/AnimationCurve.h>
#include <Math/MathUtils.h>
#include <stdint.h>

AnimationCurve<sm::Vec3>* RibbonCurveDestination::CreateCurve(
	const sm::Vec3& basePosition,
	const sm::Vec3& normal,
	SceneElement::Path* path,
	int endKeyIndex,
	float spread,
	float maxDelay)
{	
	static Randomizer random;

	const std::vector<SceneElement::Key*>& keys = path->Keys;

	float endTime = path->Keys[path->Keys.size() - 1]->Time;
	float timeScale = random.GetFloat(1.0f, (endTime + maxDelay) / endTime);

	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

	int keysCount = path->Keys.size();

	for (uint32_t i = 0; i < keysCount - 2; i++)
	{
		curve->AddKeyframe(keys[i]->Time * timeScale, path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
	}

	float firstMoveDistance = random.GetFloat(0.5f, 2.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	curve->AddKeyframe(keys[keysCount - 2]->Time * timeScale, firstMovePosition);
	curve->AddKeyframe(keys[keysCount - 1]->Time * timeScale, basePosition);

	curve->SmoothTangents();

	curve->GetKeyframe(curve->GetKeysCount() - 1).LeftTangent = 0.0f;
	curve->GetKeyframe(curve->GetKeysCount() - 1).RightTangent = 0.0f;

	return curve;
}

AnimationCurve<float>* RibbonCurveDestination::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float startTime = transformCurve->GetStartTime();
	float endTime = transformCurve->GetEndTime();

	static Randomizer random;

	curve->AddKeyframe(startTime, 0);
	curve->AddKeyframe(startTime + 1, minScale);
	curve->AddKeyframe((startTime + endTime) / 2, minScale);
	curve->AddKeyframe(endTime - 1.0f, minScale);
	curve->AddKeyframe(endTime, 1.0f);
	//curve->SmoothTangents();

	return curve;
}
