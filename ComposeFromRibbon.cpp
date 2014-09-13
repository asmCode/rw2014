#include "ComposeFromRibbon.h"
#include "TriangledMesh.h"
#include "UniqueTriangledMesh.h"
#include "Triangle.h"
#include "DemoUtils.h"
#include "DebugUtils.h"
#include "SceneElement/Path.h"
#include "SceneElement/Key.h"
#include <Math/Animation/QuarticOut.h>
#include <Utils/Randomizer.h>
#include <Graphics/Interpolators/TCBInterpolator.h>
#include <Math/Animation/AnimationCurve.h>
#include <Math/Animation/QuadOut.h>
#include <Math/Animation/LinearCurve.h>
#include <Math/Animation/Custom/BlinkCurve.h>

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
	float spread,
	float maxDelay)
{		
	static Randomizer random;

	const std::vector<SceneElement::Key*>& keys = path->Keys;

	float endTime = path->Keys[path->Keys.size() - 1]->Time;
	float timeScale = random.GetFloat(1.0f, (endTime + maxDelay) / endTime);

	AnimationCurve<sm::Vec3>* curve = new AnimationCurve<sm::Vec3>();

	int keysCount = path->Keys.size();

	for (uint32_t i = endKeyIndex - 2; i < keysCount - 2; i++)
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

AnimationCurve<float>* ComposeFromRibbon::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	AnimationCurve<float>* curve = new AnimationCurve<float>();

	float startTime = transformCurve->GetStartTime();
	float endTime = transformCurve->GetEndTime();

	static Randomizer random;

	float startTimeShift = random.GetFloat(transformCurve->GetKeyframe(0).Time, transformCurve->GetKeyframe(1).Time);

	curve->AddKeyframe(startTimeShift, 0.0f);
	curve->AddKeyframe(startTimeShift + 3.0f, minScale);
	curve->AddKeyframe((endTime + startTime) / 2.0f, minScale);
	curve->AddKeyframe(endTime - 3.0f, minScale);
	curve->AddKeyframe(endTime, 1.0f);
	//curve->SmoothTangents();

	return curve;
}

void ComposeFromRibbon::Update(float time, float deltaTime)
{
	this->TrianglesRibbon::Update(time, deltaTime);
}

void ComposeFromRibbon::ProcessTriangle(float time, int i)
{
	/*
	float timeToFinish = m_trianglesData[i]->Curve->GetEndTime() - time;
	if (timeToFinish < 0.0f)
		return;

	if (timeToFinish < 1.0f)
	{
		BlinkCurve<float, QuadOut<float>, LinearCurve<float>> curve(0.1f);
		m_triangledMesh->SetGlowPower(i, curve.Evaluate(0, 1, timeToFinish));
	}
	*/

	float timeAfterFinish = time - m_trianglesData[i]->Curve->GetEndTime();
	timeAfterFinish = MathUtils::Clamp(timeAfterFinish, 0.0f, 1.0f);

	//if (timeToFinish < 1.0f)
	{
		//BlinkCurve<float, QuadOut<float>, LinearCurve<float>> curve(1.0f);
		QuadOut<float> curve;
		m_triangledMesh->SetGlowPower(i, curve.Evaluate(0, 1, timeAfterFinish / 0.2f));
	}
}
