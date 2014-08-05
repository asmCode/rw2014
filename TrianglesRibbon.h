#pragma once

#include <Math/Vec3.h>
#include <Math/Animation/AnimationCurve.h>
#include <vector>

class TriangledMesh;
class Triangle;

class TrianglesRibbon
{
public:
	TrianglesRibbon();
	virtual ~TrianglesRibbon();

	void Initialize(
		TriangledMesh* triangledMesh,
		std::vector<sm::Vec3>& path,
		float startTime,
		float duration);

	void Update(float time, float deltaTime);

protected:
	struct TriangleData
	{
		Triangle* Triangle;
		sm::Vec3 AxisPivotTriangle[3];
		sm::Vec3 BasePosition;
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

	int m_trianglesCount;
	TriangleData** m_trianglesData;
};
