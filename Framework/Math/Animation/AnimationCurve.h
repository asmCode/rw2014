#pragma once

#include "Keyframe.h"
#include <vector>
#include <stdint.h>

template <typename T>
class AnimationCurve
{
public:
	void AddKeyframe(float time, const T& value, const T& leftTangent = T(0), const T& rightTangent = T(0))
	{
		Keyframe<T> keyframe;
		keyframe.Time = time;
		keyframe.Value = value;
		keyframe.LeftTangent = leftTangent;
		keyframe.RightTangent = rightTangent;

		m_keyframesVector.push_back(keyframe);
	}

	T Evaluate(float time, int* keyframeIndex = NULL) const
	{
		if (time <= GetStartTime())
			return m_keyframesVector[0].Value;
		else if (time >= GetEndTime())
			return m_keyframesVector[m_keyframesVector.size() - 1].Value;

		int leftKeyframeIndex = FindKeyframes(time);

		Keyframe<T> k1 = m_keyframesVector[leftKeyframeIndex];
		Keyframe<T> k2 = m_keyframesVector[leftKeyframeIndex + 1];

		time = (time - k1.Time) / (k2.Time - k1.Time);

		float t2 = time * time;
		float t3 = t2 * time;

		float a = 2.0f * t3 - 3.0f * t2 + 1.0f;
		float b = t3 - 2.0f * t2 + time;
		float c = -2.0f * t3 + 3.0f * t2;
		float d = t3 - t2;

		return
			k1.Value * a +
			k1.RightTangent * (k2.Time - k1.Time) * b+
			k2.Value * c +
			k2.LeftTangent * (k2.Time - k1.Time) * d;
	}

	float GetStartTime() const
	{
		assert(m_keyframesVector.size() > 0);

		return m_keyframesVector[0].Time;
	}

	float GetEndTime() const
	{
		assert(m_keyframesVector.size() > 0);

		return m_keyframesVector[m_keyframesVector.size() - 1].Time;
	}

	void SmoothTangents()
	{
		if (m_keyframesVector.size() < 2)
			return;

		Keyframe<T> lk;
		Keyframe<T> k;
		Keyframe<T> rk;
		T tangent;

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

	int GetKeysCount() const
	{
		return m_keyframesVector.size();
	}

	Keyframe<T>& GetKeyframe(int index)
	{
		return m_keyframesVector[index];
	}

private:
	std::vector<Keyframe<T>> m_keyframesVector;

	// returns index of left keyframe
	int FindKeyframes(float time) const
	{
#if 1
		assert(time > GetStartTime() && time < GetEndTime());

		int index = 0;
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

		return index;
#else
		for (int i = 0; i < m_keyframesVector.size(); i++)
		{
			if (m_keyframesVector[i].Time > time)
				return i - 1;
		}

		return -1;
#endif
	}
};
