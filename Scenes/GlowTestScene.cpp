#include "GlowTestScene.h"
#include "../DecomposeToRibbon.h"
#include "../Renderable.h"
#include "../GameObject.h"
#include "../UniqueTriangledMesh.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>

GlowTestScene::GlowTestScene()
{

}

GlowTestScene::~GlowTestScene()
{

}

bool GlowTestScene::Initialize()
{
	Model* teapotModel = Content::Instance->Get<Model>("teapot");

	std::vector<sm::Vec3> path;
	path.push_back(sm::Vec3(0, 0, 0));
	path.push_back(sm::Vec3(0, 0, 0.1f));

	DecomposeToRibbon* decompose = new DecomposeToRibbon();
	decompose->Initialize(teapotModel->m_meshParts[0], path, 0, 0);

	//m_teapot = new GameObject();
	//m_teapot->SetRenderable(new Renderable(decompose->GetMesh(), NULL));
	//m_gameObjects.push_back(m_teapot);

	m_renderables.push_back(new Renderable(decompose->GetMesh(), NULL));

	for (int i = 0; i < decompose->GetMesh()->GetTrianglesCount(); i++)
	{
		sm::Matrix transform =
			sm::Matrix::TranslateMatrix(decompose->GetMesh()->GetBasePosition(i)) *
			decompose->GetMesh()->GetBaseRotation(i);

		decompose->GetMesh()->SetTriangleTransform(i, transform);
	}

	return true;
}

bool GlowTestScene::Update(float time, float deltaTime)
{
	return true;
}
