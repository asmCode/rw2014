#include "SkinnedGlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include "../SkinnedMesh.h"
#include "../SkinnedRibbonMesh.h"
#include <Graphics/Shader.h>

SkinnedGlowTransparencySpecullar::SkinnedGlowTransparencySpecullar(Shader* shader, SkinnedMesh* skinnedMesh) :
	BaseGlowTransparencySpecullar(shader),
	m_mesh(skinnedMesh),
	m_meshRibbon(NULL)
{
}

SkinnedGlowTransparencySpecullar::SkinnedGlowTransparencySpecullar(Shader* shader, SkinnedRibbonMesh* skinnedRibbonMesh) :
	BaseGlowTransparencySpecullar(shader),
	m_mesh(NULL),
	m_meshRibbon(skinnedRibbonMesh)
{
}

void SkinnedGlowTransparencySpecullar::SetupRenderState()
{
	this->BaseGlowTransparencySpecullar::SetupRenderState();

	if (m_mesh != NULL)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
	}

	if (m_meshRibbon != NULL)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);
		glEnableVertexAttribArray(8);
	}
}

void SkinnedGlowTransparencySpecullar::SetupShader()
{
	this->BaseGlowTransparencySpecullar::SetupShader();

	/*for (int i = 0; i < m_mesh->GetBonesCount(); i++)
		m_mesh->BoneTransforms[i] = sm::Matrix::Identity;*/

	if (m_mesh != NULL)
	{
		assert(m_mesh->GetBonesCount() <= 41);
		m_shader->SetMatrixParameter("u_boneTransform", m_mesh->GetBonesCount(), m_mesh->BoneTransforms);
	}

	if (m_meshRibbon != NULL)
	{
		assert(m_meshRibbon->GetBonesCount() <= 41);
		m_shader->SetMatrixParameter("u_boneTransform", m_meshRibbon->GetBonesCount(), m_meshRibbon->BoneTransforms);
	}
}
