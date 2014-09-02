#include "StaticGlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include <Graphics/Shader.h>

StaticGlowTransparencySpecullar::StaticGlowTransparencySpecullar(Shader* shader)
{
	m_shader = shader;
}

bool StaticGlowTransparencySpecullar::IsOpacity() const
{
	return true;
}

void StaticGlowTransparencySpecullar::SetupRenderState()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMask(true, true, true, true);
	glDepthMask(false);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);	
}

void StaticGlowTransparencySpecullar::SetupShader()
{
	m_shader->UseProgram();
	m_shader->SetMatrixParameter("u_viewProjMatrix", DrawingRoutines::GetViewProjMatrix());
}
