#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <stdint.h>

class TriangleDataTransformGlow;

class TriangledMesh : public BaseMesh
{
public:
	TriangledMesh();
	virtual ~TriangledMesh();

	virtual void Draw();

	//TriangleDataColor* GetTrianglesData() const;
	uint32_t GetTrianglesCount() const;

	void SetTriangleTransform(int index, const sm::Matrix& transform);
	void SetGlowPower(float glowPower);
	void SetGlowPower(int index, float glowPower);

protected:
	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	int m_trianglesCount;
	TriangleDataTransformGlow* m_triangles;

	virtual void Apply();
	void CreateVertexDataBuffer();
};
