#pragma once

#include "TriangledMesh.h"
#include "TriangleDataTransformGlow.h"
#include <Math/Vec2.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
#include <Utils/Randomizer.h>
#include <GL/glew.h>

TriangledMesh::TriangledMesh() :
	m_vertexBufferId(0),
	m_vertexDataBufferId(0),
	m_indexBufferId(0),
	m_trianglesCount(0),
	m_triangles(NULL)
{
}

TriangledMesh::~TriangledMesh()
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

void TriangledMesh::CreateVertexDataBuffer()
{
	// TODO: bedzie trzeba przeniesc inicjalizacji klas pochodnych

	m_triangles = new TriangleDataTransformGlow[m_trianglesCount * 3];

	for (uint32_t i = 0; i < m_trianglesCount * 3; i++)
	{
		m_triangles[i].Transform = sm::Matrix::Identity;
		m_triangles[i].GlowPower = 0.5f;
	}

	glGenBuffers(1, &m_vertexDataBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataTransformGlow)* m_trianglesCount * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangledMesh::Apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataTransformGlow)* m_trianglesCount * 3, m_triangles, GL_DYNAMIC_DRAW);

	int baseVertexDataIndex = 1;
	for (int i = 0; i < 4; i++)
	{ 
		glVertexAttribPointer(baseVertexDataIndex + i, 4, GL_FLOAT, false, sizeof(TriangleDataTransformGlow), reinterpret_cast<void*>(sizeof(sm::Vec4) * i));
	}

	glVertexAttribPointer(5, 1, GL_FLOAT, false, sizeof(TriangleDataTransformGlow), reinterpret_cast<void*>(sizeof(sm::Vec4) * 4));
}

void TriangledMesh::Draw()
{
	Apply();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glDrawElements(GL_TRIANGLES, 3 * m_trianglesCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//TriangleDataColor* TriangledMesh::GetTrianglesData() const
//{
//	return m_triangles;
//}

uint32_t TriangledMesh::GetTrianglesCount() const
{
	return m_trianglesCount;
}

void TriangledMesh::SetTriangleTransform(int index, const sm::Matrix& transform)
{
	TriangleDataTransformGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->Transform = transform;
	(trianglesPointer + 1)->Transform = transform;
	(trianglesPointer + 2)->Transform = transform;
}

void TriangledMesh::SetGlowPower(float glowPower)
{
	for (uint32_t i = 0; i < m_trianglesCount; i++)
		SetGlowPower(i, glowPower);
}

void TriangledMesh::SetGlowPower(int index, float glowPower)
{
	TriangleDataTransformGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->GlowPower = glowPower;
	(trianglesPointer + 1)->GlowPower = glowPower;
	(trianglesPointer + 2)->GlowPower = glowPower;
}
