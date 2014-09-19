#pragma once

#include <Math\Vec3.h>
#include <Math\Quat.h>
#include <Math\Matrix.h>
#include <vector>

#include "../Math/Animation/AnimationCurve.h"

class AnimationData
{
public:
	int id;
	std::string nodeName;

	sm::Matrix worldTMInv;

	sm::Vec3 localPos;
	sm::Quat localRot;
	sm::Vec3 localScale;

	AnimationCurve<sm::Vec3> *pos;
	AnimationCurve<sm::Quat> *rot;
	AnimationCurve<sm::Vec3> *scale;

	std::vector<AnimationData*> subAnims;

	float m_animLength;

	std::vector<AnimationData*> *m_flattenedChilds;
	void FlattenChilds(std::vector<AnimationData*> &flattenChilds);

public:
	AnimationData(void);
	~AnimationData(void);

	AnimationData *GetAnimationById(int id);
	AnimationData *GetAnimationByNodeName(const std::string &name);
	float GetAnimLength();
	float GetAnimLengthById(int id);
	float GetAnimStartById(int id);
};
