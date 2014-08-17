#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <Math/Animation/AnimationCurve.h>
#include <vector>

class UniqueTriangledMesh;
class MeshPart;
class TriangleDataColor;
class BaseMesh;

class TrianglesRibbon
{
public:
	TrianglesRibbon();
	virtual ~TrianglesRibbon();

	void Initialize(
		MeshPart* meshPart,
		std::vector<sm::Vec3>& path,
		float startTime,
		float duration);

	void Update(float time, float deltaTime);

	BaseMesh* GetMesh() const;

protected:
	struct TriangleData
	{
		sm::Matrix BaseTransform;
		float Time;

		int LastKeyframeIndex;
		AnimationCurve<sm::Vec3>* Curve;
		AnimationCurve<float>* ScaleCurve;
	};

	virtual AnimationCurve<sm::Vec3>* CreateCurve(
		const sm::Vec3& basePosition,
		const sm::Vec3& normal,
		std::vector<sm::Vec3>& path,
		float startTime,
		float duration) = 0;

	virtual AnimationCurve<float>* CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve) = 0;

	UniqueTriangledMesh* m_triangledMesh;
	int m_trianglesCount;
	TriangleData** m_trianglesData;
};
