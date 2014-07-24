#include "DecomposeAndFly.h"
#include "TriangledMesh.h"
#include "Triangle.h"

DecomposeAndFly::DecomposeAndFly()
{
}

DecomposeAndFly::~DecomposeAndFly()
{
}

void DecomposeAndFly::Initialize(TriangledMesh* triangledMesh)
{
	Triangle** triangles = triangledMesh->GetTriangles();
	m_trianglesCount = triangledMesh->GetTrianglesCount();

	m_trianglesData = new TriangleData*[m_trianglesCount];

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i] = new TriangleData();
		m_trianglesData[i]->BasePosition = triangles[i]->Center;
		m_trianglesData[i]->Triangle = triangles[i];
		m_trianglesData[i]->Time = 0.0f;
		m_trianglesData[i]->Duration = 5.0f;
	}
}

void DecomposeAndFly::Update(float time, float deltaTime)
{
	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i]->Triangle->Vertices[0] += m_trianglesData[i]->Triangle->Center.GetNormalized() * deltaTime * 0.2f;
	}
}
