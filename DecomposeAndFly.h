#pragma once

#include <Math/Vec3.h>
#include <Graphics/Interpolators/IInterpolator.h>

class TriangledMesh;
class Triangle;

class DecomposeAndFly
{
public:
	DecomposeAndFly();
	~DecomposeAndFly();

	void Initialize(TriangledMesh* triangledMesh);
	void Update(float time, float deltaTime);

private:
	struct TriangleData
	{
		Triangle* Triangle;
		sm::Vec3 AxisPivotTriangle[3];
		sm::Vec3 BasePosition;
		sm::Vec3 TargetPosition;
		float Time;
		float Duration;

		IInterpolator<sm::Vec3>* Curve;
	};

	IInterpolator<sm::Vec3>* CreateCurve(const sm::Vec3& basePosition, const sm::Vec3& normal);

	int m_trianglesCount;
	TriangleData** m_trianglesData;
};
