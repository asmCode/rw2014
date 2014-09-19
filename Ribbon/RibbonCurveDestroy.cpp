#include "RibbonCurveDestroy.h"
#include <Utils/Randomizer.h>
#include "../SceneElement/Path.h"
#include "../SceneElement/Key.h"
#include "../DemoUtils.h"
#include <Math/MathUtils.h>

AnimationCurve<sm::Vec3>* CalcFallCurve(const sm::Vec3& position)
{
	return NULL;
}

AnimationCurve<sm::Vec3>* RibbonCurveDestroy::CreateCurve(
	const sm::Vec3& basePosition,
	const sm::Vec3& normal,
	SceneElement::Path* path,
	int endKeyIndex,
	float spread,
	float maxDelay)
{
	static Randomizer random;

	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

	sm::Vec3 velocity = DemoUtils::GetRandomVector();
	velocity.y = MathUtils::Abs(velocity.y);
	velocity *= random.GetFloat(0.3f, 6.0f);

	float duration = path->Keys[1]->Time - path->Keys[0]->Time;

	float time = path->Keys[0]->Time + basePosition.y * 0.05f * duration + random.GetFloat(0.0f, 1.0f);
	float timeStep = 0.1f;
	sm::Vec3 gravity(0.0f, 9.8f, 0.0);

	sm::Vec3 position = basePosition;

	while (position.y > 0.0f)
	{
		curve->AddKeyframe(time, position);

		position += velocity * timeStep;
		velocity -= gravity * timeStep;

		time += timeStep;
	}

	return curve;
}

AnimationCurve<float>* RibbonCurveDestroy::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float s = transformCurve->GetStartTime();
	float e = transformCurve->GetEndTime();

	static Randomizer random;

	curve->AddKeyframe(s, 1.0f);
	curve->AddKeyframe((s + e) / 2, 0.5f);
	curve->AddKeyframe(e, 0.5f);
	curve->AddKeyframe(e + 3, 0.0f);
	//curve->SmoothTangents();

	return curve;
}
