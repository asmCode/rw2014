#pragma once

#include <Math/Vec3.h>
#include <Graphics/Interpolators/IInterpolator.h>
#include <vector>

class TriangledMesh;
class Triangle;

class DecomposeAndFly
{
public:
	DecomposeAndFly();
	~DecomposeAndFly();

	void Initialize(
		TriangledMesh* triangledMesh,
		std::vector<sm::Vec3>& path,
		float startTime,
		float duration,
		bool morphIn);

	void Update(float time, float deltaTime);

private:
	struct TriangleData
	{
		Triangle* Triangle;
		sm::Vec3 AxisPivotTriangle[3];
		sm::Vec3 BasePosition;
		float Time;

		int LastKeyframeIndex;
		IInterpolator<sm::Vec3>* Curve;
		IInterpolator<float>* ScaleCurve;
	};

	IInterpolator<sm::Vec3>* CreateCurve(
		const sm::Vec3& basePosition,
		const sm::Vec3& normal,
		std::vector<sm::Vec3>& path,
		float startTime,
		float duration,
		bool a);

	IInterpolator<float>* CreateScaleCurve(float startTime, float endTime, bool a);

	int m_trianglesCount;
	TriangleData** m_trianglesData;
};
