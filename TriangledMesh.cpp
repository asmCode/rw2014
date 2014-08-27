#pragma once

#include "TriangledMesh.h"
#include "TriangleDataColorGlow.h"
#include <Math/Vec2.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
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

	m_triangles = new TriangleDataColorGlow[m_trianglesCount * 3];
	
	for (uint16_t i = 0; i < m_trianglesCount * 3; i++)
	{
		m_triangles[i].Transform = sm::Matrix::IdentityMatrix();

		m_triangles[i].Color.Set(0.7f, 0.2f, 0.8f, 0.5f);

		if (i >= 1000)
			m_triangles[i].GlowPower = 0.0f;
		else
			m_triangles[i].GlowPower = 2.0f;
	}

	glGenBuffers(1, &m_vertexDataBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangledMesh::Apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColorGlow)* m_trianglesCount * 3, m_triangles, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(TriangleDataColorGlow), 0);

	int baseVertexDataIndex = 2;
	for (int i = 0; i < 4; i++)
	{ 
		glVertexAttribPointer(baseVertexDataIndex + i, 4, GL_FLOAT, false, sizeof(TriangleDataColorGlow), reinterpret_cast<void*>(sizeof(sm::Vec4) + sizeof(sm::Vec4) * i));
	}

	glVertexAttribPointer(6, 1, GL_FLOAT, false, sizeof(TriangleDataColorGlow), reinterpret_cast<void*>(sizeof(sm::Vec4) + sizeof(sm::Vec4) * 4));
}

void TriangledMesh::Draw()
{
	Apply();

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0, m_trianglesCount);

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

void TriangledMesh::SetTriangleData(int index, const sm::Matrix& transform, const sm::Vec4& color)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->Transform = transform;
	trianglesPointer->Color = color;
	
	memcpy(trianglesPointer + 1, trianglesPointer, sizeof(TriangleDataColorGlow)* 2);
}

void TriangledMesh::SetTriangleTransform(int index, const sm::Matrix& transform)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->Transform = transform;
	(trianglesPointer + 1)->Transform = transform;
	(trianglesPointer + 2)->Transform = transform;
}

void TriangledMesh::SetTriangleColor(int index, const sm::Vec4& color)
{
	TriangleDataColorGlow* trianglesPointer = m_triangles + index * 3;

	trianglesPointer->Color = color;
	(trianglesPointer + 1)->Color = color;
	(trianglesPointer + 2)->Color = color;
}
