#include "Scene03.h"
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

void Scene03::InitializeSubScene()
{
	if (!SceneLoader::LoadFromFile(this, "Scene03"))
	{
		assert(false);
		return;
	}
}

bool Scene03::Update(float time, float deltaTime)
{
	time -= GetStartTime();

	this->BaseScene::Update(time, deltaTime);

	return true;
}

float Scene03::GetStartTime() const
{
	return 25 + 37;
}

float Scene03::GetEndTime() const
{
	assert(false);
	return 0.0f;
}
