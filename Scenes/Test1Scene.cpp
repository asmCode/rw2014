#include "Test1Scene.h"
#include "../DecomposeToRibbon.h"
#include "../ComposeFromRibbon.h"
#include "../GameObject.h"
#include "../UniqueTriangledMesh.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>

#include "../Renderable.h"

bool Test1Scene::Initialize()
{
	m_decomposeAndFly = new DecomposeToRibbon();
	m_composeFromRibbon = new ComposeFromRibbon();

	std::vector<sm::Vec3> path;
	path.push_back(sm::Vec3(0.0f, 30.0f, 0.0f));
	path.push_back(sm::Vec3(30.0f, 30.0f, 0.0f));
	path.push_back(sm::Vec3(40.0f, 20.0f, 5.0f));
	path.push_back(sm::Vec3(20.0f, 20.0f, -5.0f));

	m_decomposeAndFly->Initialize(
		Content::Instance->Get<Model>("teapot")->m_meshParts[0],
		path,
		3.0f,
		8.0f);

	path.clear();
	path.push_back(sm::Vec3(40.0f, 20.0f, 5.0f));
	path.push_back(sm::Vec3(20.0f, 20.0f, -5.0f));
	path.push_back(sm::Vec3(10.0f, 10.0f, 10.0f));
	path.push_back(sm::Vec3(-10.0f, -10.0f, 20.0f));

	m_composeFromRibbon->Initialize(
		Content::Instance->Get<Model>("sphere")->m_meshParts[0],
		path,
		10.0f,
		6.0f);

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("SpecularBlur");
	assert(glowSpecullarShader != NULL);

	Material* material = new GlowTransparencySpecullar(glowSpecullarShader);

	m_renderables.push_back(new Renderable(m_decomposeAndFly->GetMesh(), material));
	m_renderables.push_back(new Renderable(m_composeFromRibbon->GetMesh(), material));

	return true;
}

bool Test1Scene::Update(float time, float deltaTime)
{
	m_decomposeAndFly->Update(time, deltaTime);
	m_composeFromRibbon->Update(time, deltaTime);

	return true;
}
