#pragma once

#include "Keyframe.h"
#include <vector>

class AnimationCurve
{
public:
	void AddKeyframe(float time, float value, float leftTangent = 0.0f, float rightTangent = 0.0f);
	float Evaluate(float time, int* keyframeIndex = NULL) const;
	float GetStartTime() const;
	float GetEndTime() const;

	void SmoothTangents();

	int GetKeysCount() const;
	Keyframe& GetKeyframe(int index);

private:
	std::vector<Keyframe> m_keyframesVector;

	// returns index of left keyframe
	int FindKeyframes(float time) const;
};
