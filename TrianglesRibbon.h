#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <Math/Animation/AnimationCurve.h>
#include <vector>

class UniqueTriangledMesh;
class MeshPart;
class IRibbonCurveBuilder;
class ITriangleModificator;

namespace SceneElement { class Path; }

class TrianglesRibbon
{
public:
	struct TriangleData
	{	
		int LastKeyframeIndex;
		AnimationCurve<sm::Vec3>* Curve;
		AnimationCurve<float>* ScaleCurve;

		sm::Vec3 Axis;
		float Angle;
		float AngleSpeed;
	};

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

	void SetTriangleModificator(ITriangleModificator* triangleModificator);

	virtual void Update(float time, float deltaTime);

	UniqueTriangledMesh* GetMesh() const;
	TriangleData* GetTriangleData(int index) const;

protected:
	virtual void ProcessTriangle(float time, int i) {};

	UniqueTriangledMesh* m_triangledMesh;
	int m_trianglesCount;
	TriangleData** m_trianglesData;

	ITriangleModificator* m_triangleModificator;

	float m_minScale;
};
