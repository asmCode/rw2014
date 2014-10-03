#include "GuySceneTest.h"
#include "../GameObject.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>
#include <Graphics/Animation.h>
#include "../GraphicsLog.h"

#include "../Renderable.h"

void GuySceneTest::InitializeSubScene()
{
	SceneLoader sceneLoader;

	if (!sceneLoader.LoadFromFile(this, "GuySceneTest"))
	{
		assert(false);
		return;
	}
}

bool GuySceneTest::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	return true;
}
