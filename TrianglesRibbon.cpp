#include "TrianglesRibbon.h"
#include "UniqueTriangledMesh.h"
#include "TriangleDataTransformColorGlow.h"
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
	SceneElement::Path* path,
	int endKeyIndex,
	float spread,
	float minScale,
	float maxDelay)
{
	static Randomizer random;

	m_trianglesCount = meshPart->GetVerticesCount() / 3;

	m_triangledMesh = new UniqueTriangledMesh();
	m_triangledMesh->Initialize(meshPart);

	m_trianglesData = new TriangleData*[m_trianglesCount];

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i] = new TriangleData();
		//m_trianglesData[i]->BaseTransform = m_triangledMesh->GetBaseTransform(i);
		//m_trianglesData[i]->BaseTransform = sm::Matrix::IdentityMatrix();

		m_trianglesData[i]->LastKeyframeIndex = 0;
		m_trianglesData[i]->Curve = CreateCurve(
			m_triangledMesh->GetBasePosition(i),
			(m_triangledMesh->GetBaseRotation(i) * sm::Vec3(0, 0, 1)).GetNormalized(),
			path,
			endKeyIndex,
			spread,
			maxDelay);

		m_trianglesData[i]->ScaleCurve = CreateScaleCurve(m_trianglesData[i]->Curve, minScale);
	}
}

void TrianglesRibbon::Update(float time, float deltaTime)
{
	sm::Vec3 position;
	float scale;

	DebugUtils::DrawCurve(*m_trianglesData[0]->Curve, 0.1f, sm::Vec3(0, 0, 1));

	for (int i = 0; i < m_trianglesCount; i++)
	{
		//m_trianglesData[i]->LastKeyframeIndex = m_trianglesData[i]->Curve->GetValue(time, position, m_trianglesData[i]->LastKeyframeIndex);
		position = m_trianglesData[i]->Curve->Evaluate(time);
		scale = m_trianglesData[i]->ScaleCurve->Evaluate(time);

		m_triangledMesh->SetTriangleTransform(
			i,
			sm::Matrix::TranslateMatrix(position) *
			m_triangledMesh->GetBaseRotation(i) *
			//sm::Matrix::RotateAxisMatrix(m_trianglesData[i]->Time, 0, 0, 1) *
			sm::Matrix::ScaleMatrix(scale, scale, scale));
	}
}

UniqueTriangledMesh* TrianglesRibbon::GetMesh() const
{
	return m_triangledMesh;
}
