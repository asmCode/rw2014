#include "GlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include <Graphics/Shader.h>

GlowTransparencySpecullar::GlowTransparencySpecullar(Shader* shader) :
	m_polygonMode(GL_FILL)
{
	m_shader = shader;
}

void GlowTransparencySpecullar::SetPolygonMode(PolygonMode polygonMode)
{
	switch (polygonMode)
	{
	case PolygonMode_Lines:
		m_polygonMode = GL_LINE;
		break;

	case PolygonMode_Fill:
	default:
		m_polygonMode = GL_FILL;
		break;
	}
}

bool GlowTransparencySpecullar::IsOpacity() const
{
	return true;
}

void GlowTransparencySpecullar::SetupRenderState()
{
	glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMask(true, true, true, false);
	glDepthMask(false);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	//glDisableVertexAttribArray(0);
}

void GlowTransparencySpecullar::SetupShader()
{
	m_shader->UseProgram();
	m_shader->SetMatrixParameter("u_viewProjMatrix", DrawingRoutines::GetViewProjMatrix());
}
