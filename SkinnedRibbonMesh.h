#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <map>
#include <string>
#include <stdint.h>

class TriangleDataTransformColorGlow;
class SkinnedMeshData;
class SkinnedVertex;
class Animation;

class SkinnedRibbonMesh : public BaseMesh
{
public:
	struct TriangleData
	{
		sm::Matrix BaseTransform;

		int LastKeyframeIndex;
		AnimationCurve<sm::Vec3>* Curve;
		AnimationCurve<float>* ScaleCurve;
	};

	SkinnedRibbonMesh();
	virtual ~SkinnedRibbonMesh();

	void Initialize(
		IRibbonCurveBuilder* ribbonCurveBuilder,
		MeshPart* meshPart,
		SceneElement::Path* path,
		int endKeyIndex,
		float spread,
		float minScale,
		float maxDelay);

	sm::Matrix* BoneTransforms;

	void Initialize(SkinnedMeshData* meshData);
	virtual void Draw();

	uint32_t GetTrianglesCount() const;

	void SetColor(const sm::Vec4& color);
	void SetTriangleColor(int index, const sm::Vec4& color);
	void SetGlowPower(int index, float glowPower);

	void AddAnimation(const std::string& name, Animation* animation);

	int GetBonesCount() const;

protected:
	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	int m_trianglesCount;
	TriangleDataTransformColorGlow* m_triangles;

	SkinnedMeshData* m_meshData;

	std::map<std::string, Animation*> m_animations;

	virtual void Apply();
	void CreateVertexDataBuffer();
	void CreateVertexPositionBuffer(SkinnedMeshData* meshData);
	void CreateIndexBuffer();
};
