#include "Animation.h"
#include "Model.h"
#include "Mesh.h"
#include "../Math/MathUtils.h"
#include <stdint.h>

Animation::Animation(void) :
	m_animLength(-1.0f),
	m_lastPosKeyframeIndex(0),
	m_lastRotKeyframeIndex(0),
	m_lastScaleKeyframeIndex(0),
	m_flattenedChilds(NULL)
{
	id = 0;
	mesh = NULL;
	worldTMInv = sm::Matrix::IdentityMatrix();

	pos = NULL;
	rot = NULL;
	scale = NULL;

	localPos.Set(0, 0, 0);
	localRot.RotateToQuat(0, 0, 0, 0);
	localScale.Set(1, 1, 1);

	hasOwnRotate = false;
	ownAngle = 0.0f;
	angleScale = 0.0f;
}

Animation::~Animation(void)
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

void Animation::FlattenChilds(std::vector<Animation*> &flattenChilds)
{
	flattenChilds.push_back(this);

	for (unsigned int i = 0; i < subAnims.size(); i++)
		subAnims[i]->FlattenChilds(flattenChilds);
}

void Animation::AssignModel(Model *model)
{
	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->AssignModel(model);

	if (id == 0)
		return;

	std::vector<Mesh*> &meshes = model ->GetMeshes();
	for (unsigned i = 0; i < meshes.size(); i++)
	{
		if (meshes[i] ->id == id)
		{
			mesh = meshes[i];
			return;
		}
	}
}

void Animation::AssignTransformable(ITransformable *transformable)
{
	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->AssignTransformable(transformable);

	if (id == 0)
		return;

	if (transformable->GetId() == id)
		mesh = transformable;
}

void Animation::Update(float time, const sm::Matrix &transform, float seconds)
{
	if (mesh != NULL && mesh->IsCam())
	{
		sm::Matrix tr = transform;

		sm::Vec3 posVal;
		sm::Quat rotVal;

		float angle;
		sm::Vec3 axis;

		if (pos != NULL)
			//m_lastPosKeyframeIndex = pos ->GetValue(time, posVal, m_lastPosKeyframeIndex);
			posVal = pos->Evaluate(time, &m_lastPosKeyframeIndex);
		else
			posVal = localPos;

		if (rot != NULL)
			//m_lastRotKeyframeIndex = rot ->GetValue(time, rotVal, m_lastRotKeyframeIndex);
			rotVal = rot->Evaluate(time, &m_lastRotKeyframeIndex);
		else
			rotVal = localRot;

		rotVal.Normalize();
		rotVal.QuatToRotate(angle, axis);

		tr *= sm::Matrix::TranslateMatrix(posVal);
		tr *= sm::Matrix::RotateAxisMatrix(angle, axis);

		if (mesh != NULL)
		{
			mesh ->Transform() = sm::Matrix::RotateAxisMatrix(1.5707963f, sm::Vec3(1, 0, 0));
			mesh ->Transform() *= tr.GetInversed();
			//mesh ->Transform() *= worldTMInv.GetInversed();
		}
	}
	else
	{
		sm::Matrix tr = sm::Matrix::IdentityMatrix();

		if (mesh != NULL)
			tr *= mesh->BaseTransform();

		tr *= transform;

		sm::Vec3 posVal;
		sm::Quat rotVal;
		sm::Vec3 scaleVal;

		float angle;
		sm::Vec3 axis;

		if (pos != NULL)
			//m_lastPosKeyframeIndex = pos ->GetValue(time, posVal, m_lastPosKeyframeIndex);
			posVal = pos->Evaluate(time, &m_lastPosKeyframeIndex);
		else
			posVal = localPos;

		if (rot != NULL)
			//m_lastRotKeyframeIndex = rot ->GetValue(time, rotVal, m_lastRotKeyframeIndex);
			rotVal = rot->Evaluate(time, &m_lastRotKeyframeIndex);
		else
			rotVal = localRot;

		if (scale != NULL)
			//m_lastScaleKeyframeIndex = scale ->GetValue(time, scaleVal, m_lastScaleKeyframeIndex);
			scaleVal = scale->Evaluate(time, &m_lastScaleKeyframeIndex);
		else
			scaleVal = localScale;

		rotVal.Normalize();
		rotVal.QuatToRotate(angle, axis);

		tr *= sm::Matrix::TranslateMatrix(posVal);
		tr *= sm::Matrix::RotateAxisMatrix(angle, axis);
		tr *= sm::Matrix::ScaleMatrix(scaleVal);

		m_currentNodeTransform = tr;

		//tr *= worldTMInv;

		if (mesh != NULL)
		{
			mesh ->AnimTransform() = tr;
			mesh ->Transform() = tr;
			mesh ->Transform() *= worldTMInv;
		}

		for (unsigned i = 0; i < subAnims.size(); i++)
			subAnims[i] ->Update(time, tr, seconds);
	}
}

Animation *Animation::GetAnimationById(int id)
{
	if (this->id == id)
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
	if (this->nodeName == name)
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

float Animation::GetAnimLengthById(int id)
{
	Animation *anim = GetAnimationById(id);
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

void Animation::ReplaceAnimation(Animation *sourceAnim)
{
	assert(sourceAnim != NULL);

	if (m_flattenedChilds != NULL)
		m_flattenedChilds->clear();
	else
		m_flattenedChilds = new std::vector<Animation*>();

	if (sourceAnim->m_flattenedChilds != NULL)
		sourceAnim->m_flattenedChilds->clear();
	else
		sourceAnim->m_flattenedChilds = new std::vector<Animation*>();

	FlattenChilds(*m_flattenedChilds);
	sourceAnim->FlattenChilds(*sourceAnim->m_flattenedChilds);

	for (unsigned i = 0; i < m_flattenedChilds->size(); i++)
	{
		Animation *dstChild = (*m_flattenedChilds)[i];

		for (unsigned j = 0; j < sourceAnim->m_flattenedChilds->size(); j++)
		{
			Animation *sourceChild = (*sourceAnim->m_flattenedChilds)[j];

			if ((*m_flattenedChilds)[i]->nodeName == sourceChild->nodeName)
			{
				dstChild->m_animLength = -1.0f;

				dstChild->m_lastPosKeyframeIndex = 0;
				dstChild->m_lastRotKeyframeIndex = 0;
				dstChild->m_lastScaleKeyframeIndex = 0;

				dstChild->worldTMInv = sourceChild->worldTMInv;

				dstChild->localPos = sourceChild->localPos;
				dstChild->localRot = sourceChild->localRot;
				dstChild->localScale = sourceChild->localScale;

				dstChild->pos = sourceChild->pos;
				dstChild->rot = sourceChild->rot;
				dstChild->scale = sourceChild->scale;
			}
		}
	}
}

void Animation::SetAnimationTime(float time, const sm::Matrix &parentTransform)
{
	sm::Vec3 posVal;
	sm::Quat rotVal;
	sm::Vec3 scaleVal;

	float angle;
	sm::Vec3 axis;

	if (pos != NULL)
		posVal = pos->Evaluate(time, NULL);
	else
		posVal = localPos;

	if (rot != NULL)
		rotVal = rot->Evaluate(time, NULL);
	else
		rotVal = localRot;

	if (scale != NULL)
		scaleVal = scale->Evaluate(time, NULL);
	else
		scaleVal = localScale;

	rotVal.Normalize();
	rotVal.QuatToRotate(angle, axis);

	m_currentNodeTransform = parentTransform;
	m_currentNodeTransform *= sm::Matrix::TranslateMatrix(posVal);
	m_currentNodeTransform *= sm::Matrix::RotateAxisMatrix(angle, axis);
	m_currentNodeTransform *= sm::Matrix::ScaleMatrix(scaleVal);

	if (mesh != NULL)
		mesh->AnimTransform() = m_currentNodeTransform;

	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->SetAnimationTime(time, m_currentNodeTransform);
}

Animation *Animation::FindAnimationChild(const std::string &nodeName, bool recursively)
{
	/*if (m_flattenedChilds == NULL)
	{
		m_flattenedChilds = new std::vector<Animation*>();
		FlattenChilds(*m_flattenedChilds);
	}*/

	/*for (uint32_t i = 0; i < (*m_flattenedChilds).size(); i++)
		if ((*m_flattenedChilds)[i]->nodeName == nodeName)
			return (*m_flattenedChilds)[i];

	return NULL;*/

	for (uint32_t i = 0; i < subAnims.size(); i++)
		if (nodeName == subAnims[i]->nodeName)
			return subAnims[i];

	if (recursively)
	{
		for (uint32_t i = 0; i < subAnims.size(); i++)
		{
			Animation *anim = subAnims[i]->FindAnimationChild(nodeName, true);
			if (anim != NULL)
				return anim;
		}
	}

	return NULL;
}

void Animation::MergeAnimation(Animation *merge)
{
	assert(merge != NULL);

	if (merge->nodeName == nodeName)
	{
		for (uint32_t i = 0; i < merge->subAnims.size(); i++)
		{
			Animation *anim = FindAnimationChild(merge->subAnims[i]->nodeName, false);
			if (anim == NULL)
				subAnims.push_back(merge->subAnims[i]);
		}

		for (uint32_t i = 0; i < subAnims.size(); i++)
		{
			Animation *anim = merge->FindAnimationChild(subAnims[i]->nodeName, false);
			if (anim != NULL)
				subAnims[i]->MergeAnimation(anim);
		}
	}
	
	//if (id == 0 && m_flattenedChilds != NULL)
	//	delete m_flattenedChilds; // next time it hase to be calculated once again (hierarhy has changed)
}

void Animation::ClearLastKeys()
{
	m_lastPosKeyframeIndex = 0;
	m_lastRotKeyframeIndex = 0;
	m_lastScaleKeyframeIndex = 0;

	for (uint32_t i = 0; i < subAnims.size(); i++)
		subAnims[i]->ClearLastKeys();
}