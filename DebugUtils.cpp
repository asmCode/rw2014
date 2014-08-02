#include "DebugUtils.h"
#include "GraphicsLog.h"

void DebugUtils::DrawCurve(IInterpolator<sm::Vec3>* curve, float timeStep, const sm::Vec3& color)
{
	float start = curve->GetKeyframe(0).time;
	float length = curve->GetKeyframe(curve->GetKeysCount() - 1).time;

	sm::Vec3 beg;
	sm::Vec3 end;
	curve->GetValue(start, beg, 0);

	for (float t = timeStep; t <= length; t += timeStep)
	{
		curve->GetValue(t, end, 0);

		GraphicsLog::AddSegment(beg, end, color);

		beg = end;
	};

	for (int i = 0; i < curve->GetKeysCount(); i++)
	{
		sm::Vec3 val = curve->GetKeyframe(i).value;
		GraphicsLog::AddSegment(val, val + sm::Vec3(0, 0.5f, 0), color);
	};
}

void DebugUtils::DrawCurve(AnimationCurve& curve, float timeStep, const sm::Vec3& color)
{
	float startTime = curve.GetStartTime();
	float endTime = curve.GetEndTime();

	float beg = curve.Evaluate(startTime);
	float end;

	for (float t = startTime + timeStep; t <= endTime; t += timeStep)
	{
		end = curve.Evaluate(t);

		GraphicsLog::AddSegment(sm::Vec3((t - timeStep) * 10.0f, beg * 0.01f, 0), sm::Vec3(t * 10, end * 0.01f, 0), color);

		beg = end;
	};

	for (int i = 0; i < curve.GetKeysCount(); i++)
	{
		Keyframe& k = curve.GetKeyframe(i);

		sm::Vec3 val = sm::Vec3(k.Time * 10, k.Value * 0.01f, 0);
		GraphicsLog::AddSegment(val, val + sm::Vec3(0, 0.5f, 0), color);
	};
}
