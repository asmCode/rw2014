#include "Guy.h"
#include "../Renderable.h"
#include "../Materials/SkinnedGlowTransparencySpecullar.h"
#include "../SceneElement/Path.h"
#include "../SceneElement/Key.h"
#include "../SceneElement/IntKey.h"
#include "../SkinnedMesh.h"
#include <Math/Animation/AnimationCurve.h>
#include <Graphics/Shader.h>
#include <Graphics/Animation.h>
#include <Graphics/SkinnedMeshData.h>
#include <Graphics/Animation.h>
#include <Graphics/AnimationData.h>
#include <Graphics/Content/Content.h>

Guy::Guy(const std::string& sceneName, SceneElement::GuyData* guyData) :
	m_positionCurve(NULL),
	m_mesh(NULL)
{
	assert(guyData != NULL);
	assert(guyData->Path != NULL);

	SkinnedMeshData* meshData = Content::Instance->Get<SkinnedMeshData>("guy");
	assert(meshData);

	AnimationData* walkAnimationData = Content::Instance->Get<AnimationData>("walk");
	assert(walkAnimationData != NULL);
	AnimationData* jumpAnimationData = Content::Instance->Get<AnimationData>("jump");
	assert(jumpAnimationData != NULL);

	m_animations.push_back(new Animation(walkAnimationData));
	m_animations.push_back(new Animation(jumpAnimationData));

	m_mesh = new SkinnedMesh();
	m_mesh->Initialize(meshData);
	m_mesh->AddAnimation("walk", m_animations[0]);
	m_mesh->AddAnimation("jump", m_animations[1]);

	//m_animations[1]->SetAnimationTime(0.1f, sm::Matrix::IdentityMatrix());
	m_animations[0]->Update(0.0f, sm::Matrix::IdentityMatrix(), 0.0f);

	Shader* shader = Content::Instance->Get<Shader>("Skinned");
	assert(shader != NULL);

	SkinnedGlowTransparencySpecullar* material = new SkinnedGlowTransparencySpecullar(shader, m_mesh);

	Renderable* renderable = new Renderable(m_mesh, material);

	m_renderables.push_back(renderable);

	m_positionCurve = new AnimationCurve<sm::Vec3>();
	for (uint32_t i = 0; i < guyData->Path->Keys.size(); i++)
	{
		SceneElement::Key* key = guyData->Path->Keys[i];
		m_positionCurve->AddKeyframe(key->Time, key->Position);
	}

	for (uint32_t i = 0; i < guyData->AnimationIndex.size(); i++)
	{
		SceneElement::IntKey* key = guyData->AnimationIndex[i];
		m_animationIndex.AddKeyframe(key->Time, key->Value, false);
	}
}

Guy::~Guy()
{
}

void Guy::Update(float time, float seconds)
{
	sm::Vec3 position = m_positionCurve->Evaluate(time);
	sm::Matrix baseTransform = sm::Matrix::TranslateMatrix(position);

	//if (time >= 5)
		//m_animations[0]->Update((time - 5) * 0.1f, baseTransform, seconds);

	int animationIndex;
	m_animationIndex.GetValue(time, animationIndex);

	m_animations[animationIndex]->Update(time, baseTransform, seconds);

	/*for (uint32_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->SetActive(false);*/
}
