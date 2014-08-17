#pragma once

#include "TriangledMesh.h"
#include "TriangleDataColor.h"
#include <Math/Vec2.h>
#include <Graphics/Material.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
#include <GL/glew.h>

TriangledMesh::TriangledMesh() :
	m_vertexBufferId(0),
	m_vertexDataBufferId(0),
	m_indexBufferId(0),
	m_vertexBuffer(NULL),
	m_indexBuffer(NULL),
	m_trianglesCount(0),
	m_triangles(NULL),
	m_material(NULL)
{
}

TriangledMesh::~TriangledMesh()
{
	if (m_vertexBuffer != NULL)
		delete m_vertexBuffer;
	if (m_indexBuffer != NULL)
		delete m_indexBuffer;
	if (m_triangles != NULL)
		delete m_triangles;

	if (m_vertexBufferId != 0)
		glDeleteBuffers(1, &m_vertexBufferId);
	if (m_vertexDataBufferId != 0)
		glDeleteBuffers(1, &m_vertexDataBufferId);
	if (m_indexBufferId != 0)
		glDeleteBuffers(1, &m_indexBufferId);
}

void TriangledMesh::Initialize(int trianglesCount)
{
	m_trianglesCount = trianglesCount;

	m_vertexBuffer = new float[9]
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	m_indexBuffer = new uint16_t[3] {0, 1, 2};
	
	m_triangles = new TriangleDataColor[m_trianglesCount];
	
	for (uint16_t i = 0; i < m_trianglesCount; i++)
	{
		m_triangles[i].Transform = sm::Matrix::IdentityMatrix();
		m_triangles[i].Color.Set(1, 0, 0, 1);
	}

	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 3, m_vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_vertexDataBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColor) * m_trianglesCount, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3, m_indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriangledMesh::SetMaterial(Material* material)
{
	m_material = material;
}

Material* TriangledMesh::GetMaterial() const
{
	return m_material;
}

void TriangledMesh::Apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleDataColor)* m_trianglesCount, m_triangles, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(TriangleDataColor), 0);
	glVertexAttribDivisor(1, 1);

	int baseVertexDataIndex = 2;
	for (int i = 0; i < 4; i++)
	{
		glVertexAttribPointer(baseVertexDataIndex + i, 4, GL_FLOAT, false, sizeof(TriangleDataColor), reinterpret_cast<void*>(sizeof(sm::Vec4) + sizeof(sm::Vec4) * i));
		glVertexAttribDivisor(baseVertexDataIndex + i, 1);
	}
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

TriangleDataColor* TriangledMesh::GetTrianglesData() const
{
	return m_triangles;
}

uint32_t TriangledMesh::GetTrianglesCount() const
{
	return m_trianglesCount;
}
