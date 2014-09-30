#pragma once

#include "UniqueTriangledMesh.h"
#include "TriangledMesh.h"
#include "MeshUtils.h"
#include "DemoUtils.h"
#include <Math/Vec2.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
#include <GL/glew.h>

UniqueTriangledMesh::UniqueTriangledMesh() :
	TriangledMesh(),
	m_baseRotations(NULL),
	m_basePositions(NULL)
{
}

UniqueTriangledMesh::~UniqueTriangledMesh()
{
}

void UniqueTriangledMesh::Initialize(MeshPart* meshPart)
{
	m_trianglesCount = meshPart->GetVerticesCount() / 3;

	CreateVertexPositionBuffer(meshPart);
	CreateIndexBuffer();
	CreateVertexDataBuffer();
}

void UniqueTriangledMesh::CreateVertexPositionBuffer(MeshPart* meshPart)
{
	sm::Vec3* vertexBuffer;
	MeshUtils::ExtractVertexPositionBuffer(vertexBuffer, meshPart);

	CreateBaseTransforms(vertexBuffer);
	TransformVerticesToOrigin(vertexBuffer);

	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sm::Vec3) * m_trianglesCount * 3, vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete [] vertexBuffer;
}

void UniqueTriangledMesh::CreateIndexBuffer()
{
	uint16_t* indexBuffer = new uint16_t[m_trianglesCount * 3];

	for (int i = 0; i < m_trianglesCount * 3; i++)
		indexBuffer[i] = i;

	glGenBuffers(1, &m_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3 * m_trianglesCount, indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexBuffer;
}

void UniqueTriangledMesh::CreateBaseTransforms(sm::Vec3* vertexPositions)
{
	m_baseRotations = new sm::Matrix[m_trianglesCount];
	m_basePositions = new sm::Vec3[m_trianglesCount];

	for (int i = 0; i < m_trianglesCount; i++)
	{
		sm::Matrix transform = DemoUtils::GetTriangleTransform(vertexPositions);

		m_basePositions[i].Set(transform.a[12], transform.a[13], transform.a[14]);
		transform.a[12] = transform.a[13] = transform.a[14] = 0.0f;
		m_baseRotations[i] = transform;

		vertexPositions += 3;
	}
}

const sm::Matrix& UniqueTriangledMesh::GetBaseRotation(int index) const
{
	return m_baseRotations[index];
}

const sm::Vec3& UniqueTriangledMesh::GetBasePosition(int index) const
{
	return m_basePositions[index];
}

void UniqueTriangledMesh::TransformVerticesToOrigin(sm::Vec3* vertexPositions)
{
	for (int i = 0; i < m_trianglesCount; i++)
	{
		sm::Matrix invMatrix = (sm::Matrix::TranslateMatrix(m_basePositions[i]) * m_baseRotations[i]).GetInversed();

		vertexPositions[i * 3 + 0] = invMatrix * vertexPositions[i * 3 + 0];
		vertexPositions[i * 3 + 1] = invMatrix * vertexPositions[i * 3 + 1];
		vertexPositions[i * 3 + 2] = invMatrix * vertexPositions[i * 3 + 2];
	}
}
