#include "Quad.h"

#include <GL/glew.h>
#include <assert.h>

Quad::Quad() :
	m_vboId(0)
{

}

Quad::~Quad()
{

}

bool Quad::Initialize()
{
	assert(m_vboId == 0);

	float vertices[8] =
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f,
	};

	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void Quad::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Quad::Setup()
{
	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
}

void Quad::Clean()
{
	glDisableVertexAttribArray(0);
}
