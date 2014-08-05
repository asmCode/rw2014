#pragma once

template <typename T>
class Keyframe
{
public:
	float Time;
	T Value;
	T LeftTangent;
	T RightTangent;
};
