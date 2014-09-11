#include "Scene01.h"
#include "../GameObject.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>

#include "../Renderable.h"

void Scene01::InitializeSubScene()
{
	if (!SceneLoader::LoadFromFile(this, "Scene01"))
	{
		assert(false);
		return;
	}
}

bool Scene01::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	return true;
}
