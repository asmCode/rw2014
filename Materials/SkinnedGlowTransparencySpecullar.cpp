#include "SkinnedGlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include <Graphics/Shader.h>

SkinnedGlowTransparencySpecullar::SkinnedGlowTransparencySpecullar(Shader* shader)
{
	m_shader = shader;
}

bool SkinnedGlowTransparencySpecullar::IsOpacity() const
{
	return true;
}

void SkinnedGlowTransparencySpecullar::SetupRenderState()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMask(true, true, true, true);
	glDepthMask(true);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);	
}

void SkinnedGlowTransparencySpecullar::SetupShader()
{
	m_shader->UseProgram();
	m_shader->SetMatrixParameter("u_viewProjMatrix", DrawingRoutines::GetViewProjMatrix());

	sm::Matrix bonesTransforms[41];

	bonesTransforms[0] = sm::Matrix::IdentityMatrix();
	bonesTransforms[0] = sm::Matrix::RotateAxisMatrix(3.1415f, 0, 1, 0);

	m_shader->SetMatrixParameter("u_boneTransform", 41, bonesTransforms);
}
