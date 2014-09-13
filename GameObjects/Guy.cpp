#include "Guy.h"
#include "../Renderable.h"
#include "../Materials/SkinnedGlowTransparencySpecullar.h"
#include "../SceneElement/Path.h"
#include "../SceneElement/Key.h"
#include "../SceneElement/IntKey.h"
#include "../SceneElement/Material.h"
#include "../SkinnedMesh.h"
#include <Math/Animation/AnimationCurve.h>
#include <Graphics/Shader.h>
#include <Graphics/Animation.h>
#include <Graphics/SkinnedMeshData.h>
#include <Graphics/Animation.h>
#include <Graphics/AnimationData.h>
#include <Graphics/Content/Content.h>
#include "../GraphicsLog.h"

void DrawSegment2(Animation* root)
{
	for (int i = 0; i < root->subAnims.size(); i++)
	{
		GraphicsLog::AddSegment(root->m_currentNodeTransform * sm::Vec3(0, 0, 0), root->subAnims[i]->m_currentNodeTransform * sm::Vec3(0, 0, 0));
		DrawSegment2(root->subAnims[i]);
	}
}

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
	if (guyData->Material != NULL)
		m_mesh->SetColor(sm::Vec4(guyData->Material->Diffuse, guyData->Material->Opacity));
	m_mesh->AddAnimation("walk", m_animations[0]);
	//m_mesh->AddAnimation("jump", m_animations[1]);

	//m_animations[1]->SetAnimationTime(0.1f, sm::Matrix::IdentityMatrix());
	//m_animations[0]->Update(0.0f, sm::Matrix::IdentityMatrix(), 0.0f);

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
	sm::Matrix baseTransform = sm::Matrix::TranslateMatrix(position) * sm::Matrix::ScaleMatrix(0.02f, 0.02f, 0.02f);
	//sm::Matrix baseTransform = sm::Matrix::IdentityMatrix();

	int animationIndex = 0;

	float animLength = m_animations[animationIndex]->GetAnimLength();

	//m_animations[animationIndex]->Update(time, m_mesh->m_meshData->m_worldMatrix, seconds);
	m_animations[animationIndex]->Update(fmodf(time * 1.0f, animLength), baseTransform, seconds);
	//m_animations[animationIndex]->Update(animLength * 0.8f, baseTransform, seconds);

	DrawSegment2(m_animations[0]);

	/*for (uint32_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->SetActive(false);*/
}
