#pragma once

#include <Graphics/Material.h>
#include <stdint.h>

class SkinnedMesh;

class SkinnedGlowTransparencySpecullar : public Material
{
public:
	SkinnedGlowTransparencySpecullar(Shader* shader, SkinnedMesh* skinnedMesh);

	bool IsOpacity() const;
	void SetupRenderState();
	void SetupShader();

private:
	SkinnedMesh* m_mesh;
};
