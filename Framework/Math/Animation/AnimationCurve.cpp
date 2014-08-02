#include "AnimationCurve.h"
#include <stdint.h>
#include <assert.h>

void AnimationCurve::AddKeyframe(float time, float value, float leftTangent, float rightTangent)
{
	Keyframe keyframe;
	keyframe.Time = time;
	keyframe.Value = value;
	keyframe.LeftTangent = leftTangent;
	keyframe.RightTangent = rightTangent;

	m_keyframesVector.push_back(keyframe);
}

float AnimationCurve::Evaluate(float time, int* keyframeIndex) const
{
	if (time <= GetStartTime())
		return m_keyframesVector[0].Value;
	else if (time >= GetEndTime())
		return m_keyframesVector[m_keyframesVector.size() - 1].Value;

	int leftKeyframeIndex = FindKeyframes(time);

	Keyframe k1 = m_keyframesVector[leftKeyframeIndex];
	Keyframe k2 = m_keyframesVector[leftKeyframeIndex + 1];

	time = (time - k1.Time) / (k2.Time - k1.Time);

	float t2 = time * time;
	float t3 = t2 * time;

	float a = 2.0f * t3 - 3.0f * t2 + 1.0f;
	float b = t3 - 2.0f * t2 + time;
	float c = -2.0f * t3 + 3.0f * t2;
	float d = t3 - t2;

	return
		a * k1.Value +
		b * k1.RightTangent * (k2.Time - k1.Time) +
		c * k2.Value +
		d * k2.LeftTangent * (k2.Time - k1.Time);
}

float AnimationCurve::GetStartTime() const
{
	assert(m_keyframesVector.size() > 0);

	return m_keyframesVector[0].Time;
}

float AnimationCurve::GetEndTime() const
{
	assert(m_keyframesVector.size() > 0);

	return m_keyframesVector[m_keyframesVector.size() - 1].Time;
}

void AnimationCurve::SmoothTangents()
{
	if (m_keyframesVector.size() < 2)
		return;

	Keyframe lk;
	Keyframe k;
	Keyframe rk;
	float tangent;

	for (uint32_t i = 0; i < m_keyframesVector.size(); i++)
	{
		k = m_keyframesVector[i];

		if (i == 0)
		{
			rk = m_keyframesVector[i + 1];
			tangent = (rk.Value - k.Value) / (rk.Time - k.Time);
		}
		else if (i == m_keyframesVector.size() - 1)
		{
			lk = m_keyframesVector[i - 1];
			tangent = (k.Value - lk.Value) / (k.Time - lk.Time);
		}
		else
		{
			lk = m_keyframesVector[i - 1];
			rk = m_keyframesVector[i + 1];
			tangent = ((rk.Value - k.Value) / (2 * (rk.Time - k.Time))) + ((k.Value - lk.Value) / (2 * (k.Time - lk.Time)));
		}

		m_keyframesVector[i].LeftTangent = tangent;
		m_keyframesVector[i].RightTangent = tangent;
	}
}

int AnimationCurve::FindKeyframes(float time) const
{
	assert(time > GetStartTime() && time < GetEndTime());

	/*int index = 0;
	int jump = (m_keyframesVector.size() - 0) / 2;
	int testIndex;

	while (jump > 0)
	{
		testIndex = index + jump;
		if (testIndex < (static_cast<int>(m_keyframesVector.size()) - 1) &&
			m_keyframesVector[testIndex].Time <= time)
			index = testIndex;
		else
			jump /= 2;
	}

	return index;*/

	for (int i = 0; i < m_keyframesVector.size(); i++)
	{
		if (m_keyframesVector[i].Time > time)
			return i - 1;
	}

	return -1;
}

int AnimationCurve::GetKeysCount() const
{
	return m_keyframesVector.size();
}

Keyframe& AnimationCurve::GetKeyframe(int index)
{
	return m_keyframesVector[index];
}
