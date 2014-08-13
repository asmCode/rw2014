#include "TrianglesRibbon.h"
#include "TriangledMesh.h"
#include "TriangleDataColor.h"
#include "DemoUtils.h"
#include "DebugUtils.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
#include <Math/Animation/AnimationCurve.h>

TrianglesRibbon::TrianglesRibbon()
{
}

TrianglesRibbon::~TrianglesRibbon()
{
}

void TrianglesRibbon::Initialize(
	MeshPart* meshPart,
	std::vector<sm::Vec3>& path,
	float startTime,
	float duration)
{
	static Randomizer random;

	m_trianglesCount = meshPart->GetVerticesCount() / 3;

	m_triangledMesh = new TriangledMesh();
	m_triangledMesh->Initialize(m_trianglesCount);
	TriangleDataColor* trianglesData = m_triangledMesh->GetTrianglesData();

	m_trianglesData = new TriangleData*[m_trianglesCount];

	for (int i = 0; i < m_trianglesCount; i++)
	{
		sm::Vec3 triangleVerts[3];
		triangleVerts[0] = VertexInformation::GetPosition(meshPart->GetVertices(), i * 3 + 0, meshPart->m_vertexType);
		triangleVerts[1] = VertexInformation::GetPosition(meshPart->GetVertices(), i * 3 + 1, meshPart->m_vertexType);
		triangleVerts[2] = VertexInformation::GetPosition(meshPart->GetVertices(), i * 3 + 2, meshPart->m_vertexType);

		m_trianglesData[i] = new TriangleData();
		m_trianglesData[i]->m_triangleData = &trianglesData[i];
		m_trianglesData[i]->BaseTransform = DemoUtils::GetTransform(triangleVerts);
		m_trianglesData[i]->Time = 0.0f;

		m_trianglesData[i]->LastKeyframeIndex = 0;
		m_trianglesData[i]->Curve = CreateCurve(
			m_trianglesData[i]->BaseTransform * sm::Vec3(0, 0, 0),
			(m_trianglesData[i]->BaseTransform * sm::Vec4(0, 0, 1, 0)).XYZ(),
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

		m_trianglesData[i]->m_triangleData->Transform = 
			sm::Matrix::TranslateMatrix(position) *
			sm::Matrix::ScaleMatrix(scale, scale, scale);
	}
}

BaseMesh* TrianglesRibbon::GetMesh() const
{
	return m_triangledMesh;
}
