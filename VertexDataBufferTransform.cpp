#pragma once

#include "VertexDataBufferTransform.h"
#include <GL/glew.h>
#include <stddef.h>

VertexDataBufferTransform::VertexDataBufferTransform() :
	m_bufferId(0),
	m_transforms(NULL)
{
}

VertexDataBufferTransform::~VertexDataBufferTransform()
{
	if (m_transforms != NULL)
		delete [] m_transforms;

	if (m_bufferId != 0)
		glDeleteBuffers(1, &m_bufferId);
}

void VertexDataBufferTransform::Initialize(int trianglesCount)
{
	m_trianglesCount = trianglesCount;

	m_transforms = new sm::Matrix[m_trianglesCount * 3];

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sm::Matrix) * m_trianglesCount * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexDataBufferTransform::Apply(int attribLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sm::Matrix) * m_trianglesCount * 3, m_transforms, GL_DYNAMIC_DRAW);

	for (int i = 0; i < 4; i++)
	{
		glVertexAttribPointer(
			attribLocation + i,
			4,
			GL_FLOAT,
			false,
			sizeof(sm::Matrix),
			reinterpret_cast<void*>(sizeof(sm::Vec4) * i));
	}
}

int VertexDataBufferTransform::GetTrianglesCount() const
{
	return m_trianglesCount;
}

void VertexDataBufferTransform::SetTransform(int index, const sm::Matrix& transform)
{
	sm::Matrix* transformsPointer = m_transforms + index * 3;

	*transformsPointer = transform;
	*(transformsPointer + 1) = transform;
	*(transformsPointer + 2) = transform;
}
