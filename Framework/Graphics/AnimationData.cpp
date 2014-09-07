#include "AnimationData.h"
#include "Model.h"
#include "Mesh.h"
#include "../Math/MathUtils.h"
#include <stdint.h>

AnimationData::AnimationData(void) :
	m_animLength(-1.0f),
	m_flattenedChilds(NULL)
{
	id = 0;
	worldTMInv = sm::Matrix::IdentityMatrix();

	pos = NULL;
	rot = NULL;
	scale = NULL;

	localPos.Set(0, 0, 0);
	localRot.RotateToQuat(0, 0, 0, 0);
	localScale.Set(1, 1, 1);
}

AnimationData::~AnimationData(void)
{
	if (pos != NULL)
		delete pos;

	if (rot != NULL)
		delete rot;

	if (scale != NULL)
		delete scale;

	for (unsigned i = 0; i < subAnims.size(); i++)
		delete subAnims[i];
}

void AnimationData::FlattenChilds(std::vector<AnimationData*> &flattenChilds)
{
	flattenChilds.push_back(this);

	for (unsigned int i = 0; i < subAnims.size(); i++)
		subAnims[i]->FlattenChilds(flattenChilds);
}

AnimationData *AnimationData::GetAnimationById(int id)
{
	if (this->id == id)
		return this;

	AnimationData *anim = NULL;

	for (unsigned i = 0; i < subAnims.size(); i++)
	{
		anim = subAnims[i]->GetAnimationById(id);
		if (anim != NULL)
			return anim;
	}

	return NULL;
}

AnimationData *AnimationData::GetAnimationByNodeName(const std::string &name)
{
	if (this->nodeName == name)
		return this;

	AnimationData *anim = NULL;

	for (unsigned i = 0; i < subAnims.size(); i++)
	{
		anim = subAnims[i]->GetAnimationByNodeName(name);
		if (anim != NULL)
			return anim;
	}

	return NULL;
}

float AnimationData::GetAnimLength()
{
	if (m_animLength != -1.0f)
		return m_animLength;

	float posTime = 0.0f;
	float rotTime = 0.0f;
	float scaleTime = 0.0f;

	if (pos != NULL && pos->GetKeysCount() > 0)
		posTime = pos->GetEndTime();
	if (rot != NULL && rot->GetKeysCount() > 0)
		rotTime = rot->GetEndTime();
	if (scale != NULL && scale->GetKeysCount() > 0)
		scaleTime = scale->GetEndTime();

	float maxAnimLength = 0;
	for (int i = 0; i < subAnims.size(); i++)
	{
		float length = subAnims[i]->GetAnimLength();
		if (length > maxAnimLength)
			maxAnimLength = length;
	}

	m_animLength = MathUtils::Max(MathUtils::Max(MathUtils::Max(posTime, rotTime), scaleTime), maxAnimLength);

	return m_animLength;
}

float AnimationData::GetAnimLengthById(int id)
{
	AnimationData *anim = GetAnimationById(id);
	assert(anim != NULL);

	float posTime = 0.0f;
	float rotTime = 0.0f;
	float scaleTime = 0.0f;

	if (pos != NULL && pos->GetKeysCount() > 0)
		posTime = pos->GetEndTime();
	if (rot != NULL && rot->GetKeysCount() > 0)
		rotTime = rot->GetEndTime();
	if (scale != NULL && scale->GetKeysCount() > 0)
		scaleTime = scale->GetEndTime();

	return MathUtils::Max(MathUtils::Max(posTime, rotTime), scaleTime);
}
