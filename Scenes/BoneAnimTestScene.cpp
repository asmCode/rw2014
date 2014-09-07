#include "BoneAnimTestScene.h"
#include "../GameObject.h"
#include "../Materials/SkinnedGlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Animation.h>
#include <Graphics/Content/Content.h>
#include "../GraphicsLog.h"

#include "../Renderable.h"

Animation* anim2;

#include "../SkinnedMesh.h"
#include <Graphics/SkinnedMeshData.h>

void BoneAnimTestScene::InitializeSubScene()
{
	/*if (!SceneLoader::LoadFromFile(this, "BoneAnimTestScene"))
	{
		assert(false);
		return;
	}*/

	anim2 = Content::Instance->Get<Animation>("www");
	assert(anim2 != NULL);

	anim2->SetAnimationTime(0, sm::Matrix::IdentityMatrix());

	SkinnedMeshData* meshData = Content::Instance->Get<SkinnedMeshData>("ludzik ze skinem i szkieletem");
	assert(meshData != NULL);

	SkinnedMesh* mesh = new SkinnedMesh();
	mesh->Initialize(meshData);

	Shader* shader = Content::Instance->Get<Shader>("Skinned");
	assert(shader != NULL);
	SkinnedGlowTransparencySpecullar* material = new SkinnedGlowTransparencySpecullar(shader);

	Renderable *renderable = new Renderable(mesh, material);

	m_renderables.push_back(renderable);
}

void DrawSegment(Animation* root)
{
	for (int i = 0; i < root->subAnims.size(); i++)
	{
		GraphicsLog::AddSegment(root->m_currentNodeTransform * sm::Vec3(0, 0, 0), root->subAnims[i]->m_currentNodeTransform * sm::Vec3(0, 0, 0));
		DrawSegment(root->subAnims[i]);
	}
}

bool BoneAnimTestScene::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	if (time >= 5)
	anim2->SetAnimationTime((time - 5) * 0.1f, sm::Matrix::IdentityMatrix());

	DrawSegment(anim2->subAnims[0]);

	return true;
}
