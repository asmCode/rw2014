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

void DebugUtils::DrawCurve(AnimationCurve<sm::Vec3>& curve, float timeStep, const sm::Vec3& color)
{
	float startTime = curve.GetStartTime();
	float endTime = curve.GetEndTime();

	sm::Vec3 beg = curve.Evaluate(startTime);
	sm::Vec3 end;

	for (float t = startTime + timeStep; t <= endTime; t += timeStep)
	{
		end = curve.Evaluate(t);

		GraphicsLog::AddSegment(beg, end, color);

		beg = end;
	}

	for (int i = 0; i < curve.GetKeysCount(); i++)
	{
		Keyframe<sm::Vec3>& k = curve.GetKeyframe(i);

		sm::Vec3 val = k.Value;
		GraphicsLog::AddSegment(val, val + sm::Vec3(0, 0.5f, 0), color);
		GraphicsLog::AddSegment(val, val + sm::Vec3(0.5f, 0.0f, 0), color);
	}
}
