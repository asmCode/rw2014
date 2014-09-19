#include "SkinnedGlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include "../SkinnedMesh.h"
#include <Graphics/Shader.h>

SkinnedGlowTransparencySpecullar::SkinnedGlowTransparencySpecullar(Shader* shader, SkinnedMesh* skinnedMesh) :
	BaseGlowTransparencySpecullar(shader),
	m_mesh(skinnedMesh)
{
}

void SkinnedGlowTransparencySpecullar::SetupRenderState()
{
	this->BaseGlowTransparencySpecullar::SetupRenderState();

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
	this->BaseGlowTransparencySpecullar::SetupShader();

	/*for (int i = 0; i < m_mesh->GetBonesCount(); i++)
		m_mesh->BoneTransforms[i] = sm::Matrix::IdentityMatrix();*/

	assert(m_mesh->GetBonesCount() <= 41);
	m_shader->SetMatrixParameter("u_boneTransform", m_mesh->GetBonesCount(), m_mesh->BoneTransforms);
}
