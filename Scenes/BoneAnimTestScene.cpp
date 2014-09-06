#include "BoneAnimTestScene.h"
#include "../GameObject.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>
#include "../GraphicsLog.h"

#include "../Renderable.h"

void BoneAnimTestScene::InitializeSubScene()
{
	/*if (!SceneLoader::LoadFromFile(this, "Test1Scene"))
	{
		assert(false);
		return;
	}*/
}

bool BoneAnimTestScene::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	GraphicsLog::AddSegment(sm::Vec3(0, 0, 0), sm::Vec3(10, 10, 10));

	return true;
}
