#pragma once

#include "SkinnedMesh.h"
#include "TriangleDataColorGlow.h"
#include <Graphics/SkinnedMeshData.h>
#include <Graphics/SkinnedVertex.h>
#include "MeshUtils.h"
#include <Math/Vec2.h>
#include <GL/glew.h>

SkinnedMesh::SkinnedMesh() :
	m_vertexBufferId(0),
	m_vertexDataBufferId(0),
	m_indexBufferId(0),
	m_trianglesCount(0),
	m_triangles(NULL)
{
}

SkinnedMesh::~SkinnedMesh()
{
	if (m_triangles != NULL)
		delete m_triangles;

	if (m_vertexBufferId != 0)
		glDeleteBuffers(1, &m_vertexBufferId);
	if (m_vertexDataBufferId != 0)
		glDeleteBuffers(1, &m_vertexDataBufferId);
	if (m_indexBufferId != 0)
		glDeleteBuffers(1, &m_indexBufferId);
}

void SkinnedMesh::Initialize(SkinnedMeshData* meshData)
{
	int max = 0;
	for (int i = 0; i < meshData->verticesCount; i++)
	{
		if (meshData->vertices[i].boneIndex[0] > max)
			max = meshData->vertices[i].boneIndex[0];
	}

	m_trianglesCount = meshData->verticesCount / 3;

	CreateVertexPositionBuffer(meshData);
	CreateIndexBuffer();
	CreateVertexDataBuffer();
}

void SkinnedMesh::CreateVertexPositionBuffer(SkinnedMeshData* meshData)
{
	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkinnedVertex)* m_trianglesCount * 3, meshData->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkinnedMesh::CreateIndexBuffer()
{
	uint16_t* indexBuffer = new uint16_t[m_trianglesCount * 3];

	for (int i = 0; i < m_trianglesCount * 3; i++)
		indexBuffer[i] = i;

	glGenBuffers(1, &m_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)* 3 * m_trianglesCount, indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexBuffer;
}

void SkinnedMesh::CreateVertexDataBuffer()
{
	// TODO: bedzie trzeba przeniesc inicjalizacji klas pochodnych

	m_triangles = new TriangleDataColorGlow[m_trianglesCount * 3];

	for (uint32_t i = 0; i < m_trianglesCount * 3; i++)
	{
		m_triangles[i].Color.Set(0.2f, 0.8f, 0.0f, 0.8f);
		m_triangles[i].GlowPower = 0.2f;
	}

	glGenBuffers(1, &m_vertexDataBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkinnedMesh::Apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, m_triangles, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(TriangleDataColorGlow), 0);
	glVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(TriangleDataColorGlow), reinterpret_cast<void*>(sizeof(sm::Vec4)));
}

void SkinnedMesh::Draw()
{
	Apply();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SkinnedVertex), 0);
	//glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, false, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneIndex));
	glVertexAttribIPointer(1, 4, GL_UNSIGNED_BYTE, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneIndex));
	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, weight));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glDrawElements(GL_TRIANGLES, 3 * m_trianglesCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//TriangleDataColor* SkinnedMesh::GetTrianglesData() const
//{
//	return m_triangles;
//}

uint32_t SkinnedMesh::GetTrianglesCount() const
{
	return m_trianglesCount;
}

void SkinnedMesh::SetTriangleColor(int index, const sm::Vec4& color)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->Color = color;
	(trianglesPointer + 1)->Color = color;
	(trianglesPointer + 2)->Color = color;
}

void SkinnedMesh::SetGlowPower(int index, float glowPower)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->GlowPower = glowPower;
	(trianglesPointer + 1)->GlowPower = glowPower;
	(trianglesPointer + 2)->GlowPower = glowPower;
}
