#include "BaseGlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include <Graphics/Shader.h>

BaseGlowTransparencySpecullar::BaseGlowTransparencySpecullar(Shader* shader) :
	m_worldMatrix(NULL),
	m_polygonMode(GL_FILL),
	m_glowMultiplier(1.0f)
{
	m_shader = shader;
}

void BaseGlowTransparencySpecullar::SetShader(Shader* shader)
{
	m_shader = shader;
}

void BaseGlowTransparencySpecullar::SetWorldMatrix(const sm::Matrix* worldMatrix)
{
	m_worldMatrix = worldMatrix;
}

void BaseGlowTransparencySpecullar::SetPolygonMode(PolygonMode polygonMode)
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

void BaseGlowTransparencySpecullar::SetGlowMultiplier(float glowMultiplier)
{
	m_glowMultiplier = glowMultiplier;
}

bool BaseGlowTransparencySpecullar::IsOpacity() const
{
	return true;
}

void BaseGlowTransparencySpecullar::SetupRenderState()
{
	glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMask(true, true, true, true);
	glDepthMask(false);
}

void BaseGlowTransparencySpecullar::SetupShader()
{
	m_shader->UseProgram();
	if (m_worldMatrix != NULL)
		m_shader->SetMatrixParameter("u_viewProjMatrix", DrawingRoutines::GetViewProjMatrix() * (*m_worldMatrix));
	else
		m_shader->SetMatrixParameter("u_viewProjMatrix", DrawingRoutines::GetViewProjMatrix());

	m_shader->SetParameter("u_glowMultiplier", m_glowMultiplier);
}
