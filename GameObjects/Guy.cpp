#include "Guy.h"
#include "Ribbon.h"
#include "../Renderable.h"
#include "../Materials/SkinnedGlowTransparencySpecullar.h"
#include "../SceneElement/Path.h"
#include "../SceneElement/Key.h"
#include "../SceneElement/IntKey.h"
#include "../SceneElement/Material.h"
#include "../SkinnedMesh.h"
#include "../SkinnedRibbonMesh.h"
#include <Math/Animation/AnimationCurve.h>
#include <Math/MathUtils.h>
#include <Graphics/Shader.h>
#include <Graphics/Animation.h>
#include <Graphics/SkinnedMeshData.h>
#include <Graphics/Animation.h>
#include <Graphics/AnimationData.h>
#include <Graphics/Content/Content.h>
#include "../GraphicsLog.h"
#include <UserInput/Input.h>

void DrawSegment2(Animation* root)
{
	for (int i = 0; i < root->subAnims.size(); i++)
	{
		GraphicsLog::AddSegment(root->m_currentNodeTransform * sm::Vec3(0, 0, 0), root->subAnims[i]->m_currentNodeTransform * sm::Vec3(0, 0, 0));
		DrawSegment2(root->subAnims[i]);
	}
}

Shader* shader;

Guy::Guy(const std::string& sceneName, SceneElement::GuyData* guyData) :
	m_positionCurve(NULL),
	m_mesh(NULL),
	m_meshRibbon(NULL)
{
	m_ribbonName = guyData->RibbonName;

	if (m_ribbonName.size() == 0)
	{
		assert(guyData != NULL);
		assert(guyData->Material != NULL);
		assert(guyData->Path != NULL);

		m_name = std::string("scene.guy.") + guyData->Id;

		SkinnedMeshData* meshData = Content::Instance->Get<SkinnedMeshData>("guy");
		assert(meshData);

		AnimationData* walkAnimationData = Content::Instance->Get<AnimationData>("walk");
		assert(walkAnimationData != NULL);

		m_animations.push_back(new Animation(walkAnimationData));

		m_mesh = new SkinnedMesh();
		m_mesh->Initialize(meshData);
		m_mesh->SetColor(sm::Vec4(guyData->Material->Diffuse, guyData->Material->Opacity));
		m_mesh->SetGlowPower(guyData->Material->SolidGlowPower);
		m_mesh->AddAnimation("walk", m_animations[0]);

		//m_animations[1]->SetAnimationTime(0.1f, sm::Matrix::Identity);
		//m_animations[0]->Update(0.0f, sm::Matrix::Identity, 0.0f);

		Shader* shader = Content::Instance->Get<Shader>("Skinned");
		assert(shader != NULL);

		////////////////////
		if (guyData->Material->UseSolid)
		{
			SkinnedGlowTransparencySpecullar* material = new SkinnedGlowTransparencySpecullar(shader, m_mesh);
			material->SetGlowMultiplier(guyData->Material->SolidGlowMultiplier);

			Renderable* renderable = new Renderable(m_mesh, material);
			m_renderables.push_back(renderable);
		}

		if (guyData->Material->UseWire)
		{
			SkinnedGlowTransparencySpecullar* material = new SkinnedGlowTransparencySpecullar(shader, m_mesh);
			material->SetGlowMultiplier(guyData->Material->WireGlowMultiplier);
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			Renderable* renderable = new Renderable(m_mesh, material);
			m_renderables.push_back(renderable);
		}
		///////////////////

		m_positionCurve = new AnimationCurve<sm::Vec3>();
		for (uint32_t i = 0; i < guyData->Path->Keys.size(); i++)
		{
			SceneElement::Key* key = guyData->Path->Keys[i];
			m_positionCurve->AddKeyframe(key->Time, key->Position);
		}

		m_positionCurve->SmoothTangents();

		for (uint32_t i = 0; i < guyData->AnimationIndex.size(); i++)
		{
			SceneElement::IntKey* key = guyData->AnimationIndex[i];
			m_animationIndex.AddKeyframe(key->Time, key->Value, false);
		}
	}
	else
	{
		assert(guyData != NULL);
		assert(guyData->Material != NULL);
		assert(guyData->Path != NULL);

		m_name = std::string("scene.guy.") + guyData->Id;

		SkinnedMeshData* meshData = Content::Instance->Get<SkinnedMeshData>("guy");
		assert(meshData);

		AnimationData* walkAnimationData = Content::Instance->Get<AnimationData>("walk");
		assert(walkAnimationData != NULL);

		m_animations.push_back(new Animation(walkAnimationData));

		m_meshRibbon = new SkinnedRibbonMesh();
		m_meshRibbon->Initialize(meshData);
		m_meshRibbon->SetColor(sm::Vec4(guyData->Material->Diffuse, guyData->Material->Opacity));
		m_meshRibbon->SetGlowPower(guyData->Material->SolidGlowPower);
		m_meshRibbon->AddAnimation("walk", m_animations[0]);

		//m_animations[1]->SetAnimationTime(0.1f, sm::Matrix::Identity);
		//m_animations[0]->Update(0.0f, sm::Matrix::Identity, 0.0f);

		shader = Content::Instance->Get<Shader>("SkinnedRibbon");
		assert(shader != NULL);

		////////////////////
		if (guyData->Material->UseSolid)
		{
			SkinnedGlowTransparencySpecullar* material = new SkinnedGlowTransparencySpecullar(shader, m_meshRibbon);
			material->SetGlowMultiplier(guyData->Material->SolidGlowMultiplier);

			Renderable* renderable = new Renderable(m_meshRibbon, material);
			m_renderables.push_back(renderable);
		}

		if (guyData->Material->UseWire)
		{
			SkinnedGlowTransparencySpecullar* material = new SkinnedGlowTransparencySpecullar(shader, m_meshRibbon);
			material->SetGlowMultiplier(guyData->Material->WireGlowMultiplier);
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			Renderable* renderable = new Renderable(m_meshRibbon, material);
			m_renderables.push_back(renderable);
		}
		///////////////////

		m_positionCurve = new AnimationCurve<sm::Vec3>();
		for (uint32_t i = 0; i < guyData->Path->Keys.size(); i++)
		{
			SceneElement::Key* key = guyData->Path->Keys[i];
			m_positionCurve->AddKeyframe(key->Time, key->Position);
		}

		m_positionCurve->SmoothTangents();

		for (uint32_t i = 0; i < guyData->AnimationIndex.size(); i++)
		{
			SceneElement::IntKey* key = guyData->AnimationIndex[i];
			m_animationIndex.AddKeyframe(key->Time, key->Value, false);
		}
	}
}

Guy::~Guy()
{
}

//float d;

void Guy::Update(float time, float seconds)
{
//	if (Input::GetKey(KeyCode_T))
		//d += seconds;
	//if (Input::GetKey(KeyCode_G))
//		d -= seconds;

//	d = MathUtils::Clamp(d, 0.0f, 1.0f);

	if (m_meshRibbon != NULL)
	{
		float d = 0;
		if (m_meshRibbon->m_ribbon != NULL && m_meshRibbon->m_ribbon->m_ribbonWeightCurve != NULL)
			d = m_meshRibbon->m_ribbon->m_ribbonWeightCurve->Evaluate(time);

		d = MathUtils::Clamp(d, 0.0f, 1.0f);

		shader->UseProgram();
		shader->SetParameter("u_ribbonWeight", d);

		m_meshRibbon->Update(time, seconds);
	}

	if (time < m_positionCurve->GetStartTime() ||
		time > m_positionCurve->GetEndTime())
	{
		for (uint32_t i = 0; i < m_renderables.size(); i++)
			m_renderables[i]->SetActive(false);
	}
	else
	{
		for (uint32_t i = 0; i < m_renderables.size(); i++)
			m_renderables[i]->SetActive(true);
	}

	sm::Vec3 position = m_positionCurve->Evaluate(time);
	sm::Vec3 nextPosition = m_positionCurve->Evaluate(time + 0.05f);

	sm::Vec3 direction(0, 0, 1);

	if (position != nextPosition)
		direction = (nextPosition - position).GetNormalized();

	sm::Matrix baseTransform =
		sm::Matrix::TranslateMatrix(position) *
		sm::Matrix::CreateLookAt(direction.GetReversed(), sm::Vec3(0, 1, 0)) *
		sm::Matrix::ScaleMatrix(0.02f, 0.02f, 0.02f);

	//sm::Matrix baseTransform = sm::Matrix::Identity;

	int animationIndex = 0;

	float animLength = m_animations[animationIndex]->GetAnimLength();

	//m_animations[animationIndex]->Update(time, m_mesh->m_meshData->m_worldMatrix, seconds);
	m_animations[animationIndex]->Update(fmodf(time * 1.0f, animLength), baseTransform, seconds);
	//m_animations[animationIndex]->Update(animLength * 0.8f, baseTransform, seconds);

	//DrawSegment2(m_animations[0]);

	/*for (uint32_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->SetActive(false);*/
}

void Guy::SetRibbon(Ribbon* ribbon)
{
	m_meshRibbon->SetRibbon(ribbon);
}
