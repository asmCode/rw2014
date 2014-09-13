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
	float maxTimeScale = (endTime + maxDelay) / endTime;
	float timeScale = random.GetFloat(1.0f, maxTimeScale);


	AnimationCurve<sm::Vec3>* sampleCurve = new AnimationCurve<sm::Vec3>();
	for (uint32_t i = 0; i < path->Keys.size(); i++)
		sampleCurve->AddKeyframe(path->Keys[i]->Time, path->Keys[i]->Position);
	sampleCurve->SmoothTangents();
	
	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

	int keysCount = path->Keys.size();

	spread = 0.0f;

	//float firstKeyTimeShift = keys[0]->Time * (maxTimeScale - timeScale + 1);
	float firstKeyTimeShift = keys[0]->Time + (maxDelay * (1.0f - ((timeScale - 1.0f) / (maxTimeScale - 1.0f))));
	curve->AddKeyframe(keys[0]->Time, sampleCurve->Evaluate(firstKeyTimeShift) + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
	float ww = firstKeyTimeShift - keys[0]->Time;

	bool a = true;
	for (uint32_t i = 1; i < keysCount - 2; i++)
	{
		if (firstKeyTimeShift < keys[i]->Time)
		{
			if (!a)
			{
				a = true;
				float t = keys[i]->Time * timeScale;
				float d = keys[i]->Time - firstKeyTimeShift;
				curve->AddKeyframe((keys[0]->Time + ((keys[i]->Time * timeScale )- firstKeyTimeShift)), path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
			}
			else
			{
				curve->AddKeyframe(keys[i]->Time * timeScale, path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
			}	
		}
	}

	float firstMoveDistance = random.GetFloat(5.0f, 20.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	curve->AddKeyframe(keys[keysCount - 2]->Time * timeScale, firstMovePosition);
	curve->AddKeyframe(keys[keysCount - 1]->Time * timeScale, basePosition);

	curve->SmoothTangents();

	curve->GetKeyframe(curve->GetKeysCount() - 1).LeftTangent = 0.0f;
	curve->GetKeyframe(curve->GetKeysCount() - 1).RightTangent = 0.0f;

	delete sampleCurve;

	return curve;
	/*

	static Randomizer random;

	const std::vector<SceneElement::Key*>& keys = path->Keys;

	float endTime = path->Keys[path->Keys.size() - 1]->Time;
	float timeScale = random.GetFloat(1.0f, (endTime + maxDelay) / endTime);

	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

	int keysCount = path->Keys.size();

	spread = 0.0f;

	float firstKeyTimeShift = keys[0]->Time * timeScale;
	curve->AddKeyframe(keys[0]->Time, sampleCurve->Evaluate(firstKeyTimeShift) + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));

	bool a = false;
	for (uint32_t i = 1; i < keysCount - 2; i++)
	{
		if (firstKeyTimeShift < keys[i]->Time)
		{
			if (!a)
			{
				a = true;
				float t = keys[i]->Time * timeScale;
				float d = keys[i]->Time - firstKeyTimeShift;
				curve->AddKeyframe((keys[0]->Time + ((keys[i]->Time * timeScale) - firstKeyTimeShift)), path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
			}
			else
			{
				curve->AddKeyframe(keys[i]->Time * timeScale, path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
			}
		}
	}

	float firstMoveDistance = random.GetFloat(5.0f, 20.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	curve->AddKeyframe(keys[keysCount - 2]->Time * timeScale, firstMovePosition);
	curve->AddKeyframe(keys[keysCount - 1]->Time * timeScale, basePosition);

	curve->SmoothTangents();

	curve->GetKeyframe(curve->GetKeysCount() - 1).LeftTangent = 0.0f;
	curve->GetKeyframe(curve->GetKeysCount() - 1).RightTangent = 0.0f;

	delete sampleCurve;

	return curve;
	*/
}

AnimationCurve<float>* RibbonCurveDestination::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float startTime = transformCurve->GetStartTime();
	float endTime = transformCurve->GetEndTime();

	static Randomizer random;

	curve->AddKeyframe(endTime - 3.0f, minScale);
	curve->AddKeyframe(endTime, 1.0f);
	//curve->SmoothTangents();

	return curve;
}
