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
	float spread)
{	
	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

	static Randomizer random;

	/*
	float time = startTime;

	float firstMoveDistance = random.GetFloat(1.0f, 3.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	curve->AddKeyframe(time += +random.GetFloat(0.0, 2.0f), basePosition);
	curve->AddKeyframe(time += durationPerUnit * firstMoveDistance + random.GetFloat(1.0, 2.0f), firstMovePosition);

	for (uint32_t i = 0; i < path.size(); i++)
	{
		if (i == 0)
			time += (firstMovePosition - path[0]).GetLength() * durationPerUnit;
		else
			time += (path[i] - path[i - 1]).GetLength() * durationPerUnit;

		//time += random.GetFloat(1.0f, 2.0f) + segmentDistance * durationPerUnit;

		curve->AddKeyframe(time, path[i] + DemoUtils::GetRandomVector() * random.GetFloat(0, 2.0f));
	}
	*/

	float time = 0.0f;

	float firstMoveDistance = random.GetFloat(0.5f, 2.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	curve->AddKeyframe(time += path->Keys[0]->Time + random.GetFloat(0.0f, 1.0f), basePosition);
	curve->AddKeyframe(time += (path->Keys[1]->Time - path->Keys[0]->Time) + random.GetFloat(0.0f, 1.0f), firstMovePosition);

	for (uint32_t i = 2; i < endKeyIndex; i++)
	{
		time += (path->Keys[i]->Time - path->Keys[i - 1]->Time) + random.GetFloat(0.0f, 0.2f);

		curve->AddKeyframe(time, path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
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
	//curve->AddKeyframe((endTime + startTime) / 2.0f, 0.2f);
	curve->AddKeyframe(endTime - 3.0f, minScale);
	curve->AddKeyframe(endTime, 0.0f);
	//curve->SmoothTangents();

	return curve;
}
