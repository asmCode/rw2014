#include "Test1Scene.h"
#include "../TriangledMesh.h"
#include "../DrawingRoutines.h"
#include "../BumpAlongNormal.h"
#include "../DecomposeAndFly.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Content/Content.h>
#include <GL/glew.h>

bool Test1Scene::Initialize()
{
	m_triangleShader = new BumpAlongNormal();
	m_decomposeAndFly = new DecomposeAndFly();
	m_decomposeAndFlySphere = new DecomposeAndFly();

	float vertex[] =
	{
		 0.0f,   10.0f, 0.0f,
		-10.0f, -10.0f, 0.0f,
		 10.0f, -10.0f, 0.0f,
	};

	float normal[] =
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float texcoord[] =
	{
		0, 0,
		0, 0,
		0, 0,
	};
	

	int count = 3;

	/*float* vertex = new float[count * 3];
	float* normal = new float[count * 3];
	float* texcoord = new float[count * 2];

	static Randomizer random;

	for (int i = 0; i < count; i++)
	{
		vertex[i * 3 + 0] = random.GetFloat(-100.0f, 100.0f);
		vertex[i * 3 + 1] = random.GetFloat(-100.0f, 100.0f);
		vertex[i * 3 + 2] = random.GetFloat(-100.0f, 100.0f);
	}*/

	m_triangledMesh = new TriangledMesh();
	m_triangledMesh->Initialize(Content::Instance->Get<Model>("teapot")->m_meshParts[0]);
	m_triangledMesh->SetTriangleShader(m_triangleShader);

	m_triangledMeshSphere = new TriangledMesh();
	m_triangledMeshSphere->Initialize(Content::Instance->Get<Model>("sphere")->m_meshParts[0]);

	std::vector<sm::Vec3> path;
	path.push_back(sm::Vec3(0.0f, 30.0f, 0.0f));
	path.push_back(sm::Vec3(30.0f, 30.0f, 0.0f));
	path.push_back(sm::Vec3(40.0f, 20.0f, 5.0f));

	m_decomposeAndFly->Initialize(
		m_triangledMesh,
		path,
		3.0f,
		8.0f,
		false);

	path.clear();
	path.push_back(sm::Vec3(40.0f, 20.0f, 5.0f));
	path.push_back(sm::Vec3(40.0f, 20.0f, 10.0f));
	path.push_back(sm::Vec3(0.0f, 0.0f, 0.0f));

	m_decomposeAndFlySphere->Initialize(
		m_triangledMeshSphere,
		path,
		12.0f,
		10.0f,
		true);

	return true;
}

bool Test1Scene::Update(float time, float deltaTime)
{
	m_triangledMesh->Update(time, deltaTime);
	m_triangledMeshSphere->Update(time, deltaTime);
	m_decomposeAndFly->Update(time, deltaTime);
	m_decomposeAndFlySphere->Update(time, deltaTime);

	return true;
}

bool Test1Scene::Draw(float time, float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	DrawingRoutines::DrawWithMaterial(m_triangledMesh);
	DrawingRoutines::DrawWithMaterial(m_triangledMeshSphere);

	return true;
}
