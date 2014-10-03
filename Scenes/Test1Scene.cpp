#include "Test1Scene.h"
#include "../GameObject.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>

#include "../Renderable.h"

void Test1Scene::InitializeSubScene()
{
	SceneLoader sceneLoader;

	if (!sceneLoader.LoadFromFile(this, "Test1Scene"))
	{
		assert(false);
		return;
	}
}

bool Test1Scene::Update(float time, float deltaTime)
{
	return true;
}
