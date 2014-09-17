#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <stdint.h>

class MeshPart;
class TriangleDataColorGlow;

class StaticTriangledMesh : public BaseMesh
{
public:
	StaticTriangledMesh();
	virtual ~StaticTriangledMesh();

	void Initialize(MeshPart* meshPart);
	virtual void Draw();

	//TriangleDataColor* GetTrianglesData() const;
	uint32_t GetTrianglesCount() const;

	void SetColor(const sm::Vec4& color);
	void SetTriangleColor(int index, const sm::Vec4& color);
	void SetGlowPower(int index, float glowPower);
	void SetGlowPower(float glowPower);

protected:
	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	int m_trianglesCount;
	TriangleDataColorGlow* m_triangles;

	virtual void Apply();
	void CreateVertexDataBuffer();
	void CreateVertexPositionBuffer(MeshPart* meshPart);
	void CreateIndexBuffer();
};
