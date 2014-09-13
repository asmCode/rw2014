#include "GlowTestScene.h"
#include "../Renderable.h"
#include "../GameObject.h"
#include "../UniqueTriangledMesh.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../SceneElement/Path.h"
#include "../SceneElement/Key.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Shader.h>

GlowTestScene::GlowTestScene()
{

}

GlowTestScene::~GlowTestScene()
{

}

void GlowTestScene::InitializeSubScene()
{
	/*
	Model* teapotModel = Content::Instance->Get<Model>("teapot");

	SceneElement::Path* path = new SceneElement::Path();
	path->Keys.push_back(new SceneElement::Key());
	path->Keys.push_back(new SceneElement::Key());

	DecomposeToRibbon* decompose = new DecomposeToRibbon();
	decompose->Initialize(teapotModel->m_meshParts[0], path, 0, 0.0f, 0.4f, 0.1f);

	//m_teapot = new GameObject();
	//m_teapot->SetRenderable(new Renderable(decompose->GetMesh(), NULL));
	//m_gameObjects.push_back(m_teapot);

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("SpecularBlur");
	assert(glowSpecullarShader != NULL);

	GlowTransparencySpecullar* materialFill = new GlowTransparencySpecullar(glowSpecullarShader);

	GlowTransparencySpecullar* materialLine = new GlowTransparencySpecullar(glowSpecullarShader);
	materialLine->SetPolygonMode(GlowTransparencySpecullar::PolygonMode_Lines);

	m_renderables.push_back(new Renderable(decompose->GetMesh(), materialFill));
	m_renderables.push_back(new Renderable(decompose->GetMesh(), materialLine));

	for (int i = 0; i < decompose->GetMesh()->GetTrianglesCount(); i++)
	{
		sm::Matrix transform =
			sm::Matrix::TranslateMatrix(decompose->GetMesh()->GetBasePosition(i)) *
			decompose->GetMesh()->GetBaseRotation(i);

		decompose->GetMesh()->SetTriangleTransform(i, transform);
	}
	*/
}

bool GlowTestScene::Update(float time, float deltaTime)
{
	return true;
}
