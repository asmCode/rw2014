#include "DecomposeAndFly.h"
#include "TriangledMesh.h"
#include "Triangle.h"
#include "DemoUtils.h"
#include <Math/Animation/QuarticOut.h>
#include <Utils/Randomizer.h>
#include <Graphics/Interpolators/TCBInterpolator.h>

DecomposeAndFly::DecomposeAndFly()
{
}

DecomposeAndFly::~DecomposeAndFly()
{
}

void DecomposeAndFly::Initialize(
	TriangledMesh* triangledMesh,
	std::vector<sm::Vec3>& path,
	float startTime,
	float duration,
	bool morphIn)
{
	static Randomizer random;

	Triangle** triangles = triangledMesh->GetTriangles();
	m_trianglesCount = triangledMesh->GetTrianglesCount();

	m_trianglesData = new TriangleData*[m_trianglesCount];

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i] = new TriangleData();
		m_trianglesData[i]->BasePosition = triangles[i]->Center;
		m_trianglesData[i]->Triangle = triangles[i];
		m_trianglesData[i]->AxisPivotTriangle[0] = triangles[i]->Vertices[0] - triangles[i]->Center;
		m_trianglesData[i]->AxisPivotTriangle[1] = triangles[i]->Vertices[1] - triangles[i]->Center;
		m_trianglesData[i]->AxisPivotTriangle[2] = triangles[i]->Vertices[2] - triangles[i]->Center;
		m_trianglesData[i]->Time = 0.0f;
		//m_trianglesData[i]->Duration = random.GetFloat(2, 5);
		m_trianglesData[i]->Duration = 3.0f;
		//m_trianglesData[i]->TargetPosition = m_trianglesData[i]->BasePosition + sm::Vec3(random.GetFloat(-1, 1), random.GetFloat(-1, 1), random.GetFloat(-1, 1)).GetNormalized() * random.GetFloat(0.5f, 2) + ;
		m_trianglesData[i]->TargetPosition = m_trianglesData[i]->BasePosition + m_trianglesData[i]->Triangle->Normal;

		m_trianglesData[i]->LastKeyframeIndex = 0;
		m_trianglesData[i]->Curve = CreateCurve(
			m_trianglesData[i]->BasePosition,
			m_trianglesData[i]->Triangle->Normal,
			path,
			startTime,
			duration);

		float animStartTime = m_trianglesData[i]->Curve->GetKeyframe(0).time;
		float animEndTime = m_trianglesData[i]->Curve->GetKeyframe(m_trianglesData[i]->Curve->GetKeysCount() - 1).time;

		m_trianglesData[i]->ScaleCurve = CreateScaleCurve(animStartTime, animEndTime);
	}
}

void DecomposeAndFly::Update(float time, float deltaTime)
{
	sm::Vec3 position;
	float scale;

	for (int i = 0; i < m_trianglesCount; i++)
	{
		m_trianglesData[i]->Time += deltaTime;

		m_trianglesData[i]->LastKeyframeIndex = m_trianglesData[i]->Curve->GetValue(time, position, m_trianglesData[i]->LastKeyframeIndex);
		m_trianglesData[i]->ScaleCurve->GetValue(time, scale);

		sm::Matrix transform =
			sm::Matrix::TranslateMatrix(position) *
			sm::Matrix::ScaleMatrix(scale, scale, scale);

		m_trianglesData[i]->Triangle->Vertices[0] = transform * m_trianglesData[i]->AxisPivotTriangle[0];
		m_trianglesData[i]->Triangle->Vertices[1] = transform * m_trianglesData[i]->AxisPivotTriangle[1];
		m_trianglesData[i]->Triangle->Vertices[2] = transform * m_trianglesData[i]->AxisPivotTriangle[2];
	}
}

IInterpolator<sm::Vec3>* DecomposeAndFly::CreateCurve(
	const sm::Vec3& basePosition,
	const sm::Vec3& normal,
	std::vector<sm::Vec3>& path,
	float startTime,
	float duration)
{
	TCBInterpolator<sm::Vec3>* curve = new TCBInterpolator<sm::Vec3>();

	static Randomizer random;

	float time = startTime;

	float firstMoveDistance = random.GetFloat(1.0f, 3.0f);
	sm::Vec3 firstMovePosition = basePosition + normal * firstMoveDistance;

	float pathLength = DemoUtils::GetPathLength(path);
	assert(pathLength > 0.0f);
	pathLength += firstMoveDistance;
	pathLength += (firstMovePosition - path[0]).GetLength();
	float durationPerUnit = duration / pathLength;

	curve->AddKeyframe(time += +random.GetFloat(0.0, 2.0f), basePosition, false);
	curve->AddKeyframe(time += durationPerUnit * firstMoveDistance + random.GetFloat(1.0, 3.0f), firstMovePosition, false);

	for (uint32_t i = 0; i < path.size(); i++)
	{
		if (i == 0)
			time += (firstMovePosition - path[0]).GetLength() * durationPerUnit;
		else
			time += (path[i] - path[i - 1]).GetLength() * durationPerUnit;

		//time += random.GetFloat(1.0f, 2.0f) + segmentDistance * durationPerUnit;

		curve->AddKeyframe(time, path[i] + DemoUtils::GetRandomVector() * random.GetFloat(0, 8.0f), false);
	}

	return curve;
}

IInterpolator<float>* DecomposeAndFly::CreateScaleCurve(float startTime, float endTime)
{
	TCBInterpolator<float>* curve = new TCBInterpolator<float>();

	curve->AddKeyframe(startTime, 1.0f, false);
	curve->AddKeyframe(startTime + 3.0f, 0.3f, false);
	curve->AddKeyframe(endTime - 3.0f, 0.3f, false);
	curve->AddKeyframe(endTime, 0.0f, false);

	return curve;
}