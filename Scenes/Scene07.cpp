#include "Scene07.h"
#include "../GameObject.h"
#include "../GameObjects/Static.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../Materials/BaseGlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include "../GameObject.h"
#include "../GameObjects/Static.h"
#include <UserInput/Input.h>
#include <Utils/Randomizer.h>
#include <Utils/Log.h>
#include <Math/Animation/ExpOut.h>
#include <Math/Animation/QuadIn.h>
#include <Math/Animation/QuadOut.h>
#include <Math/Animation/LinearCurve.h>
#include <Math/Animation/QuadOut.h>
#include <Math/Animation/Custom/QuadBlinkCurve.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>
#include <Graphics/Animation.h>
#include <Graphics/Content/Content.h>

#include "../StaticTriangledMesh.h"

#include "../Renderable.h"

void Scene07::InitializeSubScene()
{
	SceneLoader sceneLoader;

	if (!sceneLoader.LoadFromFile(this, "Scene07"))
	{
		assert(false);
		return;
	}

	AnimationData* animData = Content::Instance->Get<AnimationData>("Train01");
	assert(animData != NULL);

	Model* trainModel = Content::Instance->Get<Model>("Train01");
	assert(trainModel != NULL);

	m_trainAnimation = new Animation(animData);
	m_trainAnimationWire = new Animation(animData);

	for (uint32_t i = 0; i < trainModel->meshes.size(); i++)
	{
		Mesh* mesh = trainModel->meshes[i];

		Animation* anim = m_trainAnimation->GetAnimationById(mesh->GetId());
		assert(anim != NULL);

		Animation* animWire = m_trainAnimationWire->GetAnimationById(mesh->GetId());
		assert(animWire != NULL);

		Static* staticGameObject = dynamic_cast<Static*>(FindGameObject(mesh->name));
		const std::vector<Renderable*>& renderables = staticGameObject->GetRenderables();

		assert(renderables.size() == 2);

		((BaseGlowTransparencySpecullar*)renderables[0]->GetMaterial())->SetWorldMatrix(&renderables[0]->Transform);
		((BaseGlowTransparencySpecullar*)renderables[1]->GetMaterial())->SetWorldMatrix(&renderables[1]->Transform);

		anim->AttachTransformTarget(&renderables[0]->Transform, NULL);
		animWire->AttachTransformTarget(&renderables[1]->Transform, NULL);
	}

	Static* water = dynamic_cast<Static*>(FindGameObject("woda"));
	assert(water != NULL);

	m_waterShader = Content::Instance->Get<Shader>("Water");
	assert(m_waterShader != NULL);

	((GlowTransparencySpecullar*)water->GetRenderables()[0]->GetMaterial())->SetShader(m_waterShader);

	if (water->GetRenderables().size() > 1)
		((GlowTransparencySpecullar*)water->GetRenderables()[1]->GetMaterial())->SetShader(m_waterShader);
}

bool Scene07::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	m_trainAnimation->Update(time, sm::Matrix::Identity, deltaTime);
	m_trainAnimationWire->Update(time, sm::Matrix::Identity, deltaTime);

	m_waterShader->UseProgram();
	m_waterShader->SetParameter("u_time", time);

	return true;
}
