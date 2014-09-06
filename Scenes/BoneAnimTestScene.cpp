#include "BoneAnimTestScene.h"
#include "../GameObject.h"
#include "../Materials/GlowTransparencySpecullar.h"
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

void BoneAnimTestScene::InitializeSubScene()
{
	/*if (!SceneLoader::LoadFromFile(this, "Test1Scene"))
	{
		assert(false);
		return;
	}*/

	anim2 = Content::Instance->Get<Animation>("www");
	assert(anim2 != NULL);

	anim2->SetAnimationTime(0, sm::Matrix::IdentityMatrix());
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
