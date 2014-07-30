#include "DecomposeAndFly.h"
#include "TriangledMesh.h"
#include "Triangle.h"
#include <Math/Animation/QuarticOut.h>
#include <Utils/Randomizer.h>

DecomposeAndFly::DecomposeAndFly()
{
}

DecomposeAndFly::~DecomposeAndFly()
{
}

void DecomposeAndFly::Initialize(TriangledMesh* triangledMesh)
{
	static Randomizer random;

	Triangle** triangles = triangledMesh->GetTriangles();
	m_trianglesCount = triangledMesh->GetTrianglesCount();

	m_trianglesData = new TriangleData*[m_trianglesCount];

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i] = new TriangleData();
		m_trianglesData[i]->BasePosition = triangles[i]->Center;
		m_trianglesData[i]->Triangle = triangles[i];
		m_trianglesData[i]->AxisPivotTriangle[0] = triangles[i]->Vertices[0] - triangles[i]->Center;
		m_trianglesData[i]->AxisPivotTriangle[1] = triangles[i]->Vertices[1] - triangles[i]->Center;
		m_trianglesData[i]->AxisPivotTriangle[2] = triangles[i]->Vertices[2] - triangles[i]->Center;
		m_trianglesData[i]->Time = 0.0f;
		//m_trianglesData[i]->Duration = random.GetFloat(2, 5);
		m_trianglesData[i]->Duration = 3.0f;
		//m_trianglesData[i]->TargetPosition = m_trianglesData[i]->BasePosition + sm::Vec3(random.GetFloat(-1, 1), random.GetFloat(-1, 1), random.GetFloat(-1, 1)).GetNormalized() * random.GetFloat(0.5f, 2) + ;
		m_trianglesData[i]->TargetPosition = m_trianglesData[i]->BasePosition + m_trianglesData[i]->Triangle->Normal;
	}
}

void DecomposeAndFly::Update(float time, float deltaTime)
{
	QuarticOut<sm::Vec3> curve;

	sm::Vec3 position;

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i]->Time += deltaTime;

		position = curve.Evaluate(m_trianglesData[i]->BasePosition, m_trianglesData[i]->TargetPosition, m_trianglesData[i]->Time / m_trianglesData[i]->Duration);

		sm::Matrix transform = sm::Matrix::TranslateMatrix(position);

		m_trianglesData[i]->Triangle->Vertices[0] = transform * m_trianglesData[i]->AxisPivotTriangle[0];
		m_trianglesData[i]->Triangle->Vertices[1] = transform * m_trianglesData[i]->AxisPivotTriangle[1];
		m_trianglesData[i]->Triangle->Vertices[2] = transform * m_trianglesData[i]->AxisPivotTriangle[2];
	}
}
