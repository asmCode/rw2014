#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <Math/Animation/AnimationCurve.h>
#include <vector>

class UniqueTriangledMesh;
class MeshPart;
class TriangleDataColor;
class IRibbonCurveBuilder;

namespace SceneElement { class Path; }

class TrianglesRibbon
{
public:
	TrianglesRibbon();
	virtual ~TrianglesRibbon();

	void Initialize(
		IRibbonCurveBuilder* ribbonCurveBuilder,
		MeshPart* meshPart,
		SceneElement::Path* path,
		int endKeyIndex,
		float spread,
		float minScale,
		float maxDelay);

	virtual void Update(float time, float deltaTime);

	UniqueTriangledMesh* GetMesh() const;

protected:
	struct TriangleData
	{
		sm::Matrix BaseTransform;

		int LastKeyframeIndex;
		AnimationCurve<sm::Vec3>* Curve;
		AnimationCurve<float>* ScaleCurve;
	};

	virtual void ProcessTriangle(float time, int i) {};

	UniqueTriangledMesh* m_triangledMesh;
	int m_trianglesCount;
	TriangleData** m_trianglesData;
};


/* KOMPIA KODU CO ZASWIECA TROJKAT JAK JUZ SIE NZAJDZIE NA MIEJSCU
void ComposeFromRibbon::ProcessTriangle(float time, int i)
{
	float timeAfterFinish = time - m_trianglesData[i]->Curve->GetEndTime();
	timeAfterFinish = MathUtils::Clamp(timeAfterFinish, 0.0f, 1.0f);

	QuadOut<float> curve;
	m_triangledMesh->SetGlowPower(i, curve.Evaluate(0, 1, timeAfterFinish / 0.2f));
}
*/