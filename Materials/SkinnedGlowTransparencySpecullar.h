#pragma once

#include "BaseGlowTransparencySpecullar.h"
#include <stdint.h>

class SkinnedMesh;

class SkinnedGlowTransparencySpecullar : public BaseGlowTransparencySpecullar
{
public:
	SkinnedGlowTransparencySpecullar(Shader* shader, SkinnedMesh* skinnedMesh);

	void SetupRenderState();
	void SetupShader();

private:
	SkinnedMesh* m_mesh;
};
