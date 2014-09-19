#pragma once

#include <Math\Vec3.h>
#include <Math\Quat.h>
#include <Math\Matrix.h>
#include <vector>

#include "AnimationData.h"

class Animation
{
public:
	// matrix is updated every time when SetAnimationTime(); is called
	sm::Matrix m_currentNodeTransform;

	int m_lastPosKeyframeIndex;
	int m_lastRotKeyframeIndex;
	int m_lastScaleKeyframeIndex;

	sm::Matrix* m_transform;
	sm::Matrix* m_animTransform;
	AnimationData* m_animationData;

	std::vector<Animation*> subAnims;

	std::vector<Animation*> *m_flattenedChilds;
	void FlattenChilds(std::vector<Animation*> &flattenChilds);

public:
	Animation(void);
	Animation(AnimationData* data);
	~Animation(void);

	void AttachTransformTarget(sm::Matrix* transform, sm::Matrix* animTransform);
	
	void Update(float time, const sm::Matrix &transform, float seconds);

	Animation *GetAnimationById(int id);
	Animation *GetAnimationByNodeName(const std::string &name);
	float GetAnimLength();
	float GetAnimLengthById(int id);
	float GetAnimStartById(int id);

	void ClearLastKeys();

	void SetAnimationTime(float time, const sm::Matrix &parentTransform);
};
