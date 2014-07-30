#pragma once

#include <Math/Vec3.h>

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
	};

	int m_trianglesCount;
	TriangleData** m_trianglesData;
};
