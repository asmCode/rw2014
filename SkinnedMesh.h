#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
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

	void Initialize(SkinnedMeshData* meshData);
	virtual void Draw();

	uint32_t GetTrianglesCount() const;

	void SetTriangleColor(int index, const sm::Vec4& color);
	void SetGlowPower(int index, float glowPower);

protected:
	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	int m_trianglesCount;
	TriangleDataColorGlow* m_triangles;

	virtual void Apply();
	void CreateVertexDataBuffer();
	void CreateVertexPositionBuffer(SkinnedMeshData* meshData);
	void CreateIndexBuffer();
};
