#include "ComposeFromRibbon.h"
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

ComposeFromRibbon::ComposeFromRibbon()
{
}

ComposeFromRibbon::~ComposeFromRibbon()
{
}

AnimationCurve<sm::Vec3>* ComposeFromRibbon::CreateCurve(
	const sm::Vec3& basePosition,
	const sm::Vec3& normal,
	SceneElement::Path* path,
	int endKeyIndex,
	float spread)
{	
	
	static Randomizer random;

	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();
	/*
	std::vector<sm::Vec3> pathExt;

	pathExt.insert(pathExt.begin(), path.begin(), path.end());
	pathExt.push_back(basePosition + DemoUtils::GetRandomVector() * random.GetFloat(0.5f, 1.5f));
	pathExt.push_back(basePosition);

	float time = startTime;

	float pathLength = DemoUtils::GetPathLength(pathExt);
	float durationPerUnit = duration / pathLength;

	for (uint32_t i = 0; i < pathExt.size(); i++)
	{
		if (i > 0)
			time += (pathExt[i] - pathExt[i - 1]).GetLength() * durationPerUnit;

		time += random.GetFloat(0.0f, 2.0f);

		if (i != pathExt.size() - 1)
			curve->AddKeyframe(time, pathExt[i] + DemoUtils::GetRandomVector() * random.GetFloat(0, 2.0f));
		else
			curve->AddKeyframe(time, pathExt[i]);
	}
	*/

	float time = path->Keys[endKeyIndex - 2]->Time + random.GetFloat(0.0f, 2.0f);
	int keysCount = path->Keys.size();

	for (uint32_t i = endKeyIndex - 2; i < keysCount - 2; i++)
	{
		time += (path->Keys[i]->Time - path->Keys[i - 1]->Time) + random.GetFloat(0.0f, 0.5f);

		curve->AddKeyframe(time, path->Keys[i]->Position + DemoUtils::GetRandomVector() * random.GetFloat(0, spread));
	}

	float firstMoveDistance = random.GetFloat(0.5f, 2.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	curve->AddKeyframe(time += (path->Keys[keysCount - 1]->Time - path->Keys[keysCount - 2]->Time) + random.GetFloat(0.0f, 2.0f), firstMovePosition);
	curve->AddKeyframe(time += random.GetFloat(0.5f, 1.0f), basePosition);

	curve->SmoothTangents();

	curve->GetKeyframe(curve->GetKeysCount() - 1).LeftTangent = 0.0f;
	curve->GetKeyframe(curve->GetKeysCount() - 1).RightTangent = 0.0f;

	return curve;
}

AnimationCurve<float>* ComposeFromRibbon::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float startTime = transformCurve->GetStartTime();
	float endTime = transformCurve->GetEndTime();

	static Randomizer random;

	float startTimeShift = random.GetFloat(transformCurve->GetKeyframe(0).Time, transformCurve->GetKeyframe(1).Time);

	curve->AddKeyframe(startTimeShift, 0.0f);
	curve->AddKeyframe(startTimeShift + 3.0f, minScale);
	curve->AddKeyframe(endTime - 3.0f, minScale);
	curve->AddKeyframe(endTime, 1.0f);
	//curve->SmoothTangents();

	return curve;
}
