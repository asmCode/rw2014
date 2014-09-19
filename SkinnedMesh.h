#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <map>
#include <string>
#include <stdint.h>

class TriangleDataColorGlow;
class SkinnedMeshData;
class SkinnedVertex;
class Animation;

class SkinnedMesh : public BaseMesh
{
public:
	SkinnedMesh();
	virtual ~SkinnedMesh();

	sm::Matrix* BoneTransforms;

	void Initialize(SkinnedMeshData* meshData);
	virtual void Draw();

	uint32_t GetTrianglesCount() const;

	void SetColor(const sm::Vec4& color);
	void SetTriangleColor(int index, const sm::Vec4& color);
	void SetGlowPower(int index, float glowPower);
	void SetGlowPower(float glowPower);

	void AddAnimation(const std::string& name, Animation* animation);

	int GetBonesCount() const;

protected:
	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	int m_trianglesCount;
	TriangleDataColorGlow* m_triangles;

	SkinnedMeshData* m_meshData;

	std::map<std::string, Animation*> m_animations;

	virtual void Apply();
	void CreateVertexDataBuffer();
	void CreateVertexPositionBuffer(SkinnedMeshData* meshData);
	void CreateIndexBuffer();
};
