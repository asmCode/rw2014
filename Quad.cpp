#include "Quad.h"

#include <Math/Vec4.h>
#include <Math/Matrix.h>
#include <GL/glew.h>
#include <assert.h>

Quad::Quad() :
	m_vboId(0)
{

}

Quad::~Quad()
{

}

bool Quad::Initialize(int width, int height)
{
	assert(m_vboId == 0);

	sm::Matrix proj = sm::Matrix::Ortho2DMatrix(0, (float)width, 0, (float)height);

	sm::Vec4 vertices[4] =
	{
		proj * sm::Vec4(0, (float)height, 0, 1),
		proj * sm::Vec4(0, 0, 0, 1),
		proj * sm::Vec4((float)width, (float)height, 0, 1),
		proj * sm::Vec4((float)width, 0, 0, 1)
	};

	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sm::Vec4) * 4, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void Quad::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
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
}

void Quad::Clean()
{
	glDisableVertexAttribArray(0);
}
