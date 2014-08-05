#include "ComposeFromRibbon.h"
#include "TriangledMesh.h"
#include "Triangle.h"
#include "DemoUtils.h"
#include "DebugUtils.h"
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
	std::vector<sm::Vec3>& path,
	float startTime,
	float duration)
{	
	static Randomizer random;

	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

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

	curve->SmoothTangents();

	return curve;
}

AnimationCurve<float>* ComposeFromRibbon::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float startTime = transformCurve->GetStartTime();
	float endTime = transformCurve->GetEndTime();

	static Randomizer random;

	float startTimeShift = random.GetFloat(transformCurve->GetKeyframe(0).Time, transformCurve->GetKeyframe(1).Time);

	curve->AddKeyframe(startTimeShift, 0.0f);
	curve->AddKeyframe(startTimeShift + 3.0f, 0.2f);
	curve->AddKeyframe(endTime - 3.0f, 0.2f);
	curve->AddKeyframe(endTime, 1.0f);
	curve->SmoothTangents();

	return curve;
}
