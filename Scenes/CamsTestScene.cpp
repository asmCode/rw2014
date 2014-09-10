#include "CamsTestScene.h"
#include "../GameObject.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include "../AnimCameraManager.h"
#include "../Environment.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>
#include <Graphics/Animation.h>
#include <Graphics/AnimationData.h>
#include "../GraphicsLog.h"

#include "../Renderable.h"

AnimCameraManager* dupa;
Animation* camsAnim;
AnimationData* camsAnimData;

void CamsTestScene::InitializeSubScene()
{
	if (!SceneLoader::LoadFromFile(this, "CamsTestScene"))
	{
		assert(false);
		return;
	}

	camsAnimData = Content::Instance->Get<AnimationData>(m_name);
	assert(camsAnimData != NULL);

	camsAnim = new Animation(camsAnimData);

	dupa = new AnimCameraManager();
	dupa->Load(Environment::GetInstance()->GetBasePath() + "cameras\\" + m_name + ".cam", camsAnim);
}

AnimCamera* cam;

bool CamsTestScene::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	camsAnim->Update(time, sm::Matrix::IdentityMatrix(), deltaTime);
	cam = dupa->GetActiveCamera(time);

	return true;
}

ICamera* CamsTestScene::GetCamera() const
{
	return cam;
}
