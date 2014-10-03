#include "Scene04.h"
#include "../GameObject.h"
#include "../GameObjects/Static.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneLoader.h"
#include "../SkinnedRibbonMesh.h"
#include "../GameObjects/Guy.h"
#include "../GameObjects/Ribbon.h"
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
	SceneLoader sceneLoader;

	if (!sceneLoader.LoadFromFile(this, "SceneDupa"))
	{
		assert(false);
		return;
	}
	/*
	Static* water = dynamic_cast<Static*>(FindGameObject("water"));
	assert(water != NULL);

	m_waterShader = Content::Instance->Get<Shader>("Water");
	assert(m_waterShader != NULL);

	((GlowTransparencySpecullar*)water->GetRenderables()[0]->GetMaterial())->SetShader(m_waterShader);

	if (water->GetRenderables().size() > 1)
		((GlowTransparencySpecullar*)water->GetRenderables()[1]->GetMaterial())->SetShader(m_waterShader);

	Guy* guy = dynamic_cast<Guy*>(FindGameObject("scene.guy.1"));
	assert(guy);

	guy->GetRenderables()[0]->SetActive(false);
	if (guy->GetRenderables().size() > 1)
		guy->GetRenderables()[1]->SetActive(false);
	*/

	Guy* guy = dynamic_cast<Guy*>(FindGameObject("scene.guy.1"));
	assert(guy);

	if (guy->m_ribbonName.size() > 0)
	{
		Ribbon* ribbon = dynamic_cast<Ribbon*>(FindGameObject(std::string("scene.morph.dst.") + guy->m_ribbonName));
		assert(ribbon != NULL);

		guy->SetRibbon(ribbon);
	}
}

bool Scene04::Update(float time, float deltaTime)
{
	this->BaseScene::Update(time, deltaTime);

	//m_waterShader->UseProgram();
	//m_waterShader->SetParameter("u_time", time);

	return true;
}
