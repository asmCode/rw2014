#pragma once

#include "TriangledMesh.h"
#include "TriangleShader.h"
#include <Math/Vec2.h>
#include <Graphics/Material.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
#include <GL/glew.h>

TriangledMesh::TriangledMesh() :
	m_texcoordBufferId(0),
	m_normalBufferId(0),
	m_vertexBufferId(0),
	m_indexBufferId(0),
	m_vertexCount(0),
	m_vertexBuffer(NULL),
	m_normalBuffer(NULL),
	m_texcoordBuffer(NULL),
	m_indexBuffer(NULL),
	m_material(NULL),
	m_triangleShader(NULL)
{
}

TriangledMesh::~TriangledMesh()
{
	if (m_vertexBuffer != NULL)
		delete m_vertexBuffer;
	if (m_normalBuffer != NULL)
		delete m_normalBuffer;
	if (m_texcoordBuffer != NULL)
		delete m_texcoordBuffer;
	if (m_indexBuffer != NULL)
		delete m_indexBuffer;

	glDeleteBuffers(1, &m_vertexBufferId);
	glDeleteBuffers(1, &m_normalBufferId);
	glDeleteBuffers(1, &m_texcoordBufferId);
	glDeleteBuffers(1, &m_indexBufferId);
}

void TriangledMesh::Initialize(float* vertices, float* normals, float* texcoords, int count)
{
	int fullBufferSize = count * 3;

	m_vertexCount = count;
	m_vertexBuffer = new float[count * 3];
	m_normalBuffer = new float[count * 3];
	m_texcoordBuffer = new float[count * 2];
	m_indexBuffer = new uint16_t[count];

	memcpy(m_vertexBuffer, vertices, sizeof(float) * count * 3);
	memcpy(m_normalBuffer, normals, sizeof(float)* count * 3);
	memcpy(m_texcoordBuffer, texcoords, sizeof(float)* count * 2);

	for (uint16_t i = 0; i < count; i++)
		m_indexBuffer[i] = i;

	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 3 * count, m_vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_normalBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 3 * count, m_normalBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_texcoordBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_texcoordBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 2 * count, m_texcoordBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * count, m_indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriangledMesh::Initialize(MeshPart* meshPart)
{
	const void* rawVertices = meshPart->GetVertices();
	int verticesCount = meshPart->GetVerticesCount();

	float* vertices = new float[verticesCount * 3];
	float* normals = new float[verticesCount * 3];
	float* texcoords = new float[verticesCount * 2];

	for (int i = 0; i < verticesCount; i++)
	{
		sm::Vec3 vertex = VertexInformation::GetPosition(rawVertices, i, meshPart->m_vertexType);
		sm::Vec3 normal = VertexInformation::GetNormal(rawVertices, i, meshPart->m_vertexType);
		sm::Vec2 texcoord = VertexInformation::GetTexcoord0(rawVertices, i, meshPart->m_vertexType);

		vertices[i * 3 + 0] = vertex[0];
		vertices[i * 3 + 1] = vertex[1];
		vertices[i * 3 + 2] = vertex[2];

		normals[i * 3 + 0] = normal[0];
		normals[i * 3 + 1] = normal[1];
		normals[i * 3 + 2] = normal[2];
	}

	Initialize(vertices, normals, texcoords, verticesCount);
}

void TriangledMesh::SetMaterial(Material* material)
{
	m_material = material;
}

Material* TriangledMesh::GetMaterial() const
{
	return m_material;
}

void TriangledMesh::SetTriangleShader(TriangleShader* triangleShader)
{
	m_triangleShader = triangleShader;
}

void TriangledMesh::Apply()
{

}

void TriangledMesh::Update(float time, float deltaTime)
{

}

void TriangledMesh::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferId);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, m_texcoordBufferId);
	//glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_SHORT, 0);
}

