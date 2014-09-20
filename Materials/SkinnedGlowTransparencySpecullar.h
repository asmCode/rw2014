#pragma once

#include "BaseGlowTransparencySpecullar.h"
#include <stdint.h>

class SkinnedMesh;
class SkinnedRibbonMesh;

class SkinnedGlowTransparencySpecullar : public BaseGlowTransparencySpecullar
{
public:
	SkinnedGlowTransparencySpecullar(Shader* shader, SkinnedMesh* skinnedMesh);
	SkinnedGlowTransparencySpecullar(Shader* shader, SkinnedRibbonMesh* skinnedRibbonMesh);

	void SetupRenderState();
	void SetupShader();

private:
	SkinnedMesh* m_mesh;
	SkinnedRibbonMesh* m_meshRibbon;
};
