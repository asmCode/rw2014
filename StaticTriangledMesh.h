#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <stdint.h>

class MeshPart;
class TriangleDataGlow;

class StaticTriangledMesh : public BaseMesh
{
public:
	StaticTriangledMesh();
	virtual ~StaticTriangledMesh();

	void Initialize(MeshPart* meshPart);
	virtual void Draw();

	//TriangleDataColor* GetTrianglesData() const;
	uint32_t GetTrianglesCount() const;

	void SetGlowPower(int index, float glowPower);
	void SetGlowPower(float glowPower);

protected:
	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	int m_trianglesCount;
	TriangleDataGlow* m_triangles;

	virtual void Apply();
	void CreateVertexDataBuffer();
	void CreateVertexPositionBuffer(MeshPart* meshPart);
	void CreateIndexBuffer();

private:
	bool m_isDirty;
};
