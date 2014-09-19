#include "Scene02.h"
#include "../GameObject.h"
#include "../GameObjects/Static.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneLoader.h"
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
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>

#include "../StaticTriangledMesh.h"

#include "../Renderable.h"

void Scene02::InitializeSubScene()
{
	if (!SceneLoader::LoadFromFile(this, "Scene02"))
	{
		assert(false);
		return;
	}
}

bool Scene02::Update(float time, float deltaTime)
{
	time -= GetStartTime();

	this->BaseScene::Update(time, deltaTime);

	return true;
}

float Scene02::GetStartTime() const
{
	return 37.0f;
}

float Scene02::GetEndTime() const
{
	assert(false);
	return 0.0f;
}