#pragma once

#include "TriangledMesh.h"
#include <Math/Matrix.h>
#include <Math/Vec3.h>
#include <stdint.h>

class MeshPart;

class UniqueTriangledMesh : public TriangledMesh
{
public:
	UniqueTriangledMesh();
	virtual ~UniqueTriangledMesh();

	void Initialize(MeshPart* meshPart);

	const sm::Matrix& GetBaseRotation(int index) const;
	const sm::Vec3& GetBasePosition(int index) const;

private:
	sm::Matrix* m_baseRotations;
	sm::Vec3* m_basePositions;

	void CreateVertexPositionBuffer(MeshPart* meshPart);
	void CreateIndexBuffer();
	void CreateBaseTransforms(sm::Vec3* vertexPositions);
	void TransformVerticesToOrigin(sm::Vec3* vertexPositions);
};
