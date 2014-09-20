#include "Scene04.h"
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

void Scene04::InitializeSubScene()
{
	if (!SceneLoader::LoadFromFile(this, "Scene04"))
	{
		assert(false);
		return;
	}

	Static* water = dynamic_cast<Static*>(FindGameObject("water"));
	assert(water != NULL);

	m_waterShader = Content::Instance->Get<Shader>("Water");
	assert(m_waterShader != NULL);

	((GlowTransparencySpecullar*)water->GetRenderables()[0]->GetMaterial())->SetShader(m_waterShader);

	if (water->GetRenderables().size() > 1)
		((GlowTransparencySpecullar*)water->GetRenderables()[1]->GetMaterial())->SetShader(m_waterShader);
}

bool Scene04::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	m_waterShader->UseProgram();
	m_waterShader->SetParameter("u_time", time);

	return true;
}
