#include "TrianglesRibbon.h"
#include "TriangledMesh.h"
#include "Triangle.h"
#include "DemoUtils.h"
#include "DebugUtils.h"
#include <Utils/Randomizer.h>
#include <Graphics/Interpolators/TCBInterpolator.h>
#include <Math/Animation/AnimationCurve.h>

TrianglesRibbon::TrianglesRibbon()
{
}

TrianglesRibbon::~TrianglesRibbon()
{
}

void TrianglesRibbon::Initialize(
	TriangledMesh* triangledMesh,
	std::vector<sm::Vec3>& path,
	float startTime,
	float duration)
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

		m_trianglesData[i]->LastKeyframeIndex = 0;
		m_trianglesData[i]->Curve = CreateCurve(
			m_trianglesData[i]->BasePosition,
			m_trianglesData[i]->Triangle->Normal,
			path,
			startTime,
			duration);

		m_trianglesData[i]->ScaleCurve = CreateScaleCurve(m_trianglesData[i]->Curve);
	}
}

void TrianglesRibbon::Update(float time, float deltaTime)
{
	sm::Vec3 position;
	float scale;

	//DebugUtils::DrawCurve(*m_trianglesData[0]->Curve, 0.1f, sm::Vec3(0, 0, 1));

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i]->Time += deltaTime;

		//m_trianglesData[i]->LastKeyframeIndex = m_trianglesData[i]->Curve->GetValue(time, position, m_trianglesData[i]->LastKeyframeIndex);
		position = m_trianglesData[i]->Curve->Evaluate(time);
		scale = m_trianglesData[i]->ScaleCurve->Evaluate(time);

		sm::Matrix transform =
			sm::Matrix::TranslateMatrix(position) *
			sm::Matrix::ScaleMatrix(scale, scale, scale);

		m_trianglesData[i]->Triangle->Vertices[0] = transform * m_trianglesData[i]->AxisPivotTriangle[0];
		m_trianglesData[i]->Triangle->Vertices[1] = transform * m_trianglesData[i]->AxisPivotTriangle[1];
		m_trianglesData[i]->Triangle->Vertices[2] = transform * m_trianglesData[i]->AxisPivotTriangle[2];
	}
}

