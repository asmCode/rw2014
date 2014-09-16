#include "BlinkAtStartAndEnd.h"
#include "../../UniqueTriangledMesh.h"
#include "../../TrianglesRibbon.h"
#include <Math/Animation/QuadOut.h>
#include <Math/Animation/LinearCurve.h>
#include <Math/Animation/Custom/BlinkCurve.h>

BlinkAtStartAndEnd* BlinkAtStartAndEnd::m_instance;

BlinkAtStartAndEnd* BlinkAtStartAndEnd::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new BlinkAtStartAndEnd();

	return m_instance;
}

BlinkAtStartAndEnd::BlinkAtStartAndEnd()
{

}

void BlinkAtStartAndEnd::ProcessTriangle(float time, TrianglesRibbon* trianglesRibbon, int i)
{
	static float BlinkDuration = 1.0f;

	TrianglesRibbon::TriangleData *triangleData = trianglesRibbon->GetTriangleData(i);
	float startTime = triangleData->Curve->GetStartTime();
	if (time < startTime || time > (startTime + BlinkDuration))
		return;

	float blinkTimeNorm = (time - startTime) / BlinkDuration;

	TriangledMesh* triangledMesh = trianglesRibbon->GetMesh();

	BlinkCurve<float, QuadOut<float>, LinearCurve<float> > curve(0.05f);
	triangledMesh->SetGlowPower(i, curve.Evaluate(0.8f, 2, blinkTimeNorm));
}

