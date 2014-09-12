#pragma once

#include "StaticTriangledMesh.h"
#include "TriangleDataColorGlow.h"
#include "MeshUtils.h"
#include <Math/Vec2.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
#include <Utils/Randomizer.h>
#include <GL/glew.h>

StaticTriangledMesh::StaticTriangledMesh() :
	m_vertexBufferId(0),
	m_vertexDataBufferId(0),
	m_indexBufferId(0),
	m_trianglesCount(0),
	m_triangles(NULL)
{
}

StaticTriangledMesh::~StaticTriangledMesh()
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

void StaticTriangledMesh::Initialize(MeshPart* meshPart)
{
	m_trianglesCount = meshPart->GetVerticesCount() / 3;

	CreateVertexPositionBuffer(meshPart);
	CreateIndexBuffer();
	CreateVertexDataBuffer();
}

void StaticTriangledMesh::CreateVertexPositionBuffer(MeshPart* meshPart)
{
	sm::Vec3* vertexBuffer;
	MeshUtils::ExtractVertexPositionBuffer(vertexBuffer, meshPart);

	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sm::Vec3) * m_trianglesCount * 3, vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertexBuffer;
}

void StaticTriangledMesh::CreateIndexBuffer()
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

void StaticTriangledMesh::CreateVertexDataBuffer()
{
	// TODO: bedzie trzeba przeniesc inicjalizacji klas pochodnych

	m_triangles = new TriangleDataColorGlow[m_trianglesCount * 3];

	for (uint32_t i = 0; i < m_trianglesCount * 3; i++)
	{
		m_triangles[i].Color.Set(0.2f, 0.8f, 0.0f, 0.8f);
		m_triangles[i].GlowPower = 1.0f;
	}

	glGenBuffers(1, &m_vertexDataBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StaticTriangledMesh::Apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, m_triangles, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(TriangleDataColorGlow), 0);
	glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(TriangleDataColorGlow), reinterpret_cast<void*>(sizeof(sm::Vec4)));
}

void StaticTriangledMesh::Draw()
{
	Apply();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glDrawElements(GL_TRIANGLES, 3 * m_trianglesCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//TriangleDataColor* StaticTriangledMesh::GetTrianglesData() const
//{
//	return m_triangles;
//}

uint32_t StaticTriangledMesh::GetTrianglesCount() const
{
	return m_trianglesCount;
}

void StaticTriangledMesh::SetColor(const sm::Vec4& color)
{
	for (uint32_t i = 0; i < m_trianglesCount; i++)
		SetTriangleColor(i, color);
}

void StaticTriangledMesh::SetTriangleColor(int index, const sm::Vec4& color)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->Color = color;
	(trianglesPointer + 1)->Color = color;
	(trianglesPointer + 2)->Color = color;
}

void StaticTriangledMesh::SetGlowPower(int index, float glowPower)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->GlowPower = glowPower;
	(trianglesPointer + 1)->GlowPower = glowPower;
	(trianglesPointer + 2)->GlowPower = glowPower;
}
