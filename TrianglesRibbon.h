#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <Math/Animation/AnimationCurve.h>
#include <vector>

class UniqueTriangledMesh;
class MeshPart;
class TriangleDataColor;

namespace SceneElement { class Path; }

class TrianglesRibbon
{
public:
	TrianglesRibbon();
	virtual ~TrianglesRibbon();

	void Initialize(
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

	virtual AnimationCurve<sm::Vec3>* CreateCurve(
		const sm::Vec3& basePosition,
		const sm::Vec3& normal,
		SceneElement::Path* path,
		int endKeyIndex,
		float spread,
		float maxDelayOnEachNode) = 0;

	virtual void ProcessTriangle(float time, int i) {};

	virtual AnimationCurve<float>* CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale) = 0;

	UniqueTriangledMesh* m_triangledMesh;
	int m_trianglesCount;
	TriangleData** m_trianglesData;
};
