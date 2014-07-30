#include "DecomposeAndFly.h"
#include "TriangledMesh.h"
#include "Triangle.h"
#include "DemoUtils.h"
#include <Math/Animation/QuarticOut.h>
#include <Utils/Randomizer.h>
#include <Graphics/Interpolators/TCBInterpolator.h>

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

		m_trianglesData[i]->Curve = CreateCurve(m_trianglesData[i]->BasePosition, m_trianglesData[i]->Triangle->Normal);
	}
}

void DecomposeAndFly::Update(float time, float deltaTime)
{
	/*
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
	*/

	sm::Vec3 position;

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i]->Time += deltaTime;

		m_trianglesData[i]->Curve->GetValue(time, position);

		sm::Matrix transform = sm::Matrix::TranslateMatrix(position);

		m_trianglesData[i]->Triangle->Vertices[0] = transform * m_trianglesData[i]->AxisPivotTriangle[0];
		m_trianglesData[i]->Triangle->Vertices[1] = transform * m_trianglesData[i]->AxisPivotTriangle[1];
		m_trianglesData[i]->Triangle->Vertices[2] = transform * m_trianglesData[i]->AxisPivotTriangle[2];
	}
}

IInterpolator<sm::Vec3>* DecomposeAndFly::CreateCurve(const sm::Vec3& basePosition, const sm::Vec3& normal)
{
	TCBInterpolator<sm::Vec3>* curve = new TCBInterpolator<sm::Vec3>();

	static Randomizer random;

	curve->AddKeyframe(random.GetFloat(0.5f, 1.5f), basePosition, false);
	curve->AddKeyframe(random.GetFloat(2.0f, 3.0f), basePosition + normal * random.GetFloat(1, 3), false);
	curve->AddKeyframe(random.GetFloat(4.0f, 8.0f), sm::Vec3(0, 50, 0) + DemoUtils::GetRandomVector() * 4.0f, false);
	curve->AddKeyframe(random.GetFloat(9.0f, 13.0f), sm::Vec3(30, 50, 0) + DemoUtils::GetRandomVector() * 4.0f, false);
	curve->AddKeyframe(random.GetFloat(14.0f, 18.0f), sm::Vec3(0, 0, 0) + DemoUtils::GetRandomVector() * 4.0f, false);

	return curve;
}
