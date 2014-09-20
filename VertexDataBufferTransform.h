#pragma once

#include "BaseMesh.h"
#include <Math/Matrix.h>
#include <stdint.h>

class VertexDataBufferTransform
{
public:
	VertexDataBufferTransform();
	virtual ~VertexDataBufferTransform();

	void Initialize(int trianglesCount);

	void SetTransform(int triangleIndex, const sm::Matrix& transform);
	void Apply(int attribLocation);
	void Bind();

	int GetTrianglesCount() const;

protected:
	uint32_t m_bufferId;

	sm::Matrix* m_transforms;

	int m_trianglesCount;
};
