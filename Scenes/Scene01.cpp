#include "Scene01.h"
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

void Scene01::InitializeSubScene()
{
	if (!SceneLoader::LoadFromFile(this, "Scene01"))
	{
		assert(false);
		return;
	}
}

float t = 0.0f;
bool w = false;

bool Scene01::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	return true;

	if (Input::GetKeyDown(KeyCode::KeyCode_A))
	{
		w = true;
	}

	if (w)
		t += deltaTime;

	//QuadBlinkCurve d(0.1f);
	BlinkCurve<float, QuadOut<float>, LinearCurve<float>> d(0.1f);
	Static* pyramid = dynamic_cast<Static*>(FindGameObject("Pyramid003"));
	if (t >= 2.0)
		pyramid->GetMesh()->SetColor(sm::Vec4(0.3f, 0.6f, 1.0f, 0.0f));
	else
		pyramid->GetMesh()->SetColor(sm::Vec4(0.3f, 0.6f, 1.0f, d.Evaluate(0, 1, t / 2.0f)));

	if (Input::GetKeyUp(KeyCode::KeyCode_A))
	{
		Log::LogT("waginiuim");
	}


	return true;
}
