#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <stdint.h>

class TriangleDataTransformColorGlow;

class TriangledMesh : public BaseMesh
{
public:
	TriangledMesh();
	virtual ~TriangledMesh();

	virtual void Draw();

	//TriangleDataColor* GetTrianglesData() const;
	uint32_t GetTrianglesCount() const;

	void SetTriangleData(int index, const sm::Matrix& transform, const sm::Vec4& color);
	void SetTriangleTransform(int index, const sm::Matrix& transform);
	void SetColor(const sm::Vec4& color);
	void SetTriangleColor(int index, const sm::Vec4& color);
	void SetGlowPower(int index, float glowPower);

protected:
	uint32_t m_vertexBufferId;
	uint32_t m_vertexDataBufferId;
	uint32_t m_indexBufferId;

	int m_trianglesCount;
	TriangleDataTransformColorGlow* m_triangles;

	virtual void Apply();
	void CreateVertexDataBuffer();
};
