#include "RibbonCurveFullSource.h"
#include "../SceneElement/Key.h"
#include "../DemoUtils.h"
#include <Utils/Randomizer.h>
#include <Math/Animation/AnimationCurve.h>
#include <Math/MathUtils.h>
#include <stdint.h>

AnimationCurve<sm::Vec3>* RibbonCurveFullSource::CreateCurve(
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

	float firstMoveDistance = random.GetFloat(0.0f, 0.0f);
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

AnimationCurve<float>* RibbonCurveFullSource::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float startTime = transformCurve->GetStartTime();
	float endTime = transformCurve->GetEndTime();

	static Randomizer random;

	float startTimeShift = random.GetFloat(transformCurve->GetKeyframe(0).Time, transformCurve->GetKeyframe(1).Time);

	curve->AddKeyframe(startTimeShift, 1.0f);
	curve->AddKeyframe(startTimeShift + 0.5f, minScale);
	curve->AddKeyframe(startTimeShift + 0.6f, minScale);
	curve->AddKeyframe((endTime + startTime) / 2.0f, minScale);
	curve->AddKeyframe(endTime - 0.6f, minScale);
	curve->AddKeyframe(endTime - 0.5f, minScale);
	curve->AddKeyframe(endTime, 0.0f);
	//curve->SmoothTangents();

	return curve;
}
