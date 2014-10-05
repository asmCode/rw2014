#include "Animation.h"
#include "Model.h"
#include "Mesh.h"
#include "../Math/MathUtils.h"
#include <stdint.h>

Animation::Animation(void) :
	m_lastPosKeyframeIndex(0),
	m_lastRotKeyframeIndex(0),
	m_lastScaleKeyframeIndex(0),
	m_transform(NULL),
	m_animTransform(NULL),
	m_animationData(NULL),
	m_flattenedChilds(NULL)
{
}

Animation::Animation(AnimationData* data) :
	Animation()
{
	m_animationData = data;

	for (uint32_t i = 0; i < data->subAnims.size(); i++)
	{
		Animation* animation = new Animation(data->subAnims[i]);
		subAnims.push_back(animation);
	}
}

Animation::~Animation(void)
{
	for (unsigned i = 0; i < subAnims.size(); i++)
		delete subAnims[i];
}

void Animation::FlattenChilds(std::vector<Animation*> &flattenChilds)
{
	flattenChilds.push_back(this);

	for (unsigned int i = 0; i < subAnims.size(); i++)
		subAnims[i]->FlattenChilds(flattenChilds);
}

void Animation::Update(float time, const sm::Matrix &transform, float seconds)
{
	if (m_transform == NULL &&
		m_animTransform == NULL &&
		subAnims.size() == 0)
		return;

	sm::Matrix tr = transform;

	sm::Vec3 posVal;
	sm::Quat rotVal;
	sm::Vec3 scaleVal;

	float angle;
	sm::Vec3 axis;

	if (m_animationData->pos != NULL)
		//m_lastPosKeyframeIndex = pos ->GetValue(time, posVal, m_lastPosKeyframeIndex);
		posVal = m_animationData->pos->Evaluate(time, &m_lastPosKeyframeIndex);
	else
		posVal = m_animationData->localPos;

	if (m_animationData->rot != NULL)
		//m_lastRotKeyframeIndex = rot ->GetValue(time, rotVal, m_lastRotKeyframeIndex);
		rotVal = m_animationData->rot->Evaluate(time, &m_lastRotKeyframeIndex);
	else
		rotVal = m_animationData->localRot;

	if (m_animationData->scale != NULL)
		//m_lastScaleKeyframeIndex = scale ->GetValue(time, scaleVal, m_lastScaleKeyframeIndex);
		scaleVal = m_animationData->scale->Evaluate(time, &m_lastScaleKeyframeIndex);
	else
		scaleVal = m_animationData->localScale;

	rotVal.Normalize();
	rotVal.QuatToRotate(angle, axis);

	tr *= sm::Matrix::TranslateMatrix(posVal);
	tr *= sm::Matrix::RotateAxisMatrix(angle, axis);
	tr *= sm::Matrix::ScaleMatrix(scaleVal);

	m_currentNodeTransform = tr;

	if (m_animTransform != NULL)
		*m_animTransform = tr;

	if (m_transform != NULL)
		*m_transform = tr * m_animationData->worldTMInv;

	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->Update(time, tr, seconds);
}

Animation *Animation::GetAnimationById(int id)
{
	if (m_animationData->id == id)
		return this;

	Animation *anim = NULL;

	for (unsigned i = 0; i < subAnims.size(); i++)
	{
		anim = subAnims[i]->GetAnimationById(id);
		if (anim != NULL)
			return anim;
	}

	return NULL;
}

Animation *Animation::GetAnimationByNodeName(const std::string &name)
{
	if (m_animationData->nodeName == name)
		return this;

	Animation *anim = NULL;

	for (unsigned i = 0; i < subAnims.size(); i++)
	{
		anim = subAnims[i]->GetAnimationByNodeName(name);
		if (anim != NULL)
			return anim;
	}

	return NULL;
}

float Animation::GetAnimLength()
{
	return m_animationData->GetAnimLength();
}

float Animation::GetAnimLengthById(int id)
{
	return m_animationData->GetAnimLengthById(id);
}

float Animation::GetAnimStartById(int id)
{
	return m_animationData->GetAnimStartById(id);
}

void Animation::SetAnimationTime(float time, const sm::Matrix &parentTransform)
{
	sm::Vec3 posVal;
	sm::Quat rotVal;
	sm::Vec3 scaleVal;

	float angle;
	sm::Vec3 axis;

	if (m_animationData->pos != NULL)
		posVal = m_animationData->pos->Evaluate(time, NULL);
	else
		posVal = m_animationData->localPos;

	if (m_animationData->rot != NULL)
		rotVal = m_animationData->rot->Evaluate(time, NULL);
	else
		rotVal = m_animationData->localRot;

	if (m_animationData->scale != NULL)
		scaleVal = m_animationData->scale->Evaluate(time, NULL);
	else
		scaleVal = m_animationData->localScale;

	rotVal.Normalize();
	rotVal.QuatToRotate(angle, axis);

	m_currentNodeTransform = parentTransform;
	m_currentNodeTransform *= sm::Matrix::TranslateMatrix(posVal);
	m_currentNodeTransform *= sm::Matrix::RotateAxisMatrix(angle, axis);
	m_currentNodeTransform *= sm::Matrix::ScaleMatrix(scaleVal);

	if (m_animTransform != NULL)
		*m_animTransform = m_currentNodeTransform;

	if (m_transform != NULL)
		*m_transform = m_currentNodeTransform * m_animationData->worldTMInv;

	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->SetAnimationTime(time, m_currentNodeTransform);
}

void Animation::ClearLastKeys()
{
	m_lastPosKeyframeIndex = 0;
	m_lastRotKeyframeIndex = 0;
	m_lastScaleKeyframeIndex = 0;

	for (uint32_t i = 0; i < subAnims.size(); i++)
		subAnims[i]->ClearLastKeys();
}

void Animation::AttachTransformTarget(sm::Matrix* transform, sm::Matrix* animTransform)
{
	m_transform = transform;
	m_animTransform = animTransform;
}
