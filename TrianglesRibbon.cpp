#include "TrianglesRibbon.h"
#include "UniqueTriangledMesh.h"
#include "Ribbon/IRibbonCurveBuilder.h"
#include "Ribbon/Modificators/ITriangleModificator.h"
#include "DemoUtils.h"
#include "DebugUtils.h"
#include <Utils/Randomizer.h>
#include <Graphics/MeshPart.h>
#include <Math/MathUtils.h>
#include <Graphics/VertexInformation.h>
#include <Math/Animation/AnimationCurve.h>

TrianglesRibbon::TrianglesRibbon() :
	m_triangleModificator(NULL),
	m_minScale(0.4f)
{
}

TrianglesRibbon::~TrianglesRibbon()
{
}

void TrianglesRibbon::Initialize(
	IRibbonCurveBuilder* ribbinCurveBuilder,
	MeshPart* meshPart,
	SceneElement::Path* path,
	int endKeyIndex,
	float spread,
	float minScale,
	float maxDelay)
{
	assert(ribbinCurveBuilder != NULL);

	m_minScale = minScale;

	static Randomizer random;

	m_trianglesCount = meshPart->GetVerticesCount() / 3;

	m_triangledMesh = new UniqueTriangledMesh();
	m_triangledMesh->Initialize(meshPart);

	m_trianglesData = new TriangleData*[m_trianglesCount];

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i] = new TriangleData();
		//m_trianglesData[i]->BaseTransform = m_triangledMesh->GetBaseTransform(i);
		//m_trianglesData[i]->BaseTransform = sm::Matrix::Identity;

		m_trianglesData[i]->LastKeyframeIndex = 0;
		m_trianglesData[i]->Curve = ribbinCurveBuilder->CreateCurve(
			m_triangledMesh->GetBasePosition(i),
			(m_triangledMesh->GetBaseRotation(i) * sm::Vec3(0, 0, 1)).GetNormalized(),
			path,
			endKeyIndex,
			spread,
			maxDelay);

		m_trianglesData[i]->ScaleCurve = ribbinCurveBuilder->CreateScaleCurve(m_trianglesData[i]->Curve, minScale);

		m_trianglesData[i]->Axis = DemoUtils::GetRandomVector();
		m_trianglesData[i]->Angle = 0.0f;
		m_trianglesData[i]->AngleSpeed = random.GetFloat(1.0, 10.0f);
	}
}

void TrianglesRibbon::SetTriangleModificator(ITriangleModificator* triangleModificator)
{
	m_triangleModificator = triangleModificator;
}

void TrianglesRibbon::Update(float time, float deltaTime)
{
	sm::Vec3 position;
	float scale;

	//DebugUtils::DrawCurve(*m_trianglesData[0]->Curve, 0.1f, sm::Vec3(0, 0, 1));

	for (int i = 0; i < m_trianglesCount; i++)
	{
		//DebugUtils::DrawCurve(*m_trianglesData[i]->Curve, 0.1f, sm::Vec3(0, 0, 1));

		//m_trianglesData[i]->LastKeyframeIndex = m_trianglesData[i]->Curve->GetValue(time, position, m_trianglesData[i]->LastKeyframeIndex);
		position = m_trianglesData[i]->Curve->Evaluate(time);
		scale = m_trianglesData[i]->ScaleCurve->Evaluate(time);

		float normTimeOnCurve = (time - m_trianglesData[i]->Curve->GetStartTime()) / (m_trianglesData[i]->Curve->GetEndTime() - m_trianglesData[i]->Curve->GetStartTime());
		m_trianglesData[i]->Angle = (cos(normTimeOnCurve * 3.1415f * 2) - 1) * -0.5f;
		m_trianglesData[i]->Angle *= m_trianglesData[i]->AngleSpeed;

		//m_trianglesData[i]->Angle += m_trianglesData[i]->AngleSpeed * deltaTime;

		m_triangledMesh->SetTriangleTransform(
			i,
			sm::Matrix::TranslateMatrix(position) *
			m_triangledMesh->GetBaseRotation(i) *
			sm::Matrix::RotateAxisMatrix(m_trianglesData[i]->Angle * (1.0f - scale), m_trianglesData[i]->Axis) *
			sm::Matrix::ScaleMatrix(scale, scale, scale));

		if (m_triangleModificator != NULL)
			m_triangleModificator->ProcessTriangle(time, this, i);
		ProcessTriangle(time, i);
	}
}

UniqueTriangledMesh* TrianglesRibbon::GetMesh() const
{
	return m_triangledMesh;
}

TrianglesRibbon::TriangleData* TrianglesRibbon::GetTriangleData(int index) const
{
	return m_trianglesData[index];
}
