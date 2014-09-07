#pragma once

#include <Graphics/Material.h>
#include <stdint.h>

class SkinnedGlowTransparencySpecullar : public Material
{
public:
	SkinnedGlowTransparencySpecullar(Shader* shader);

	bool IsOpacity() const;
	void SetupRenderState();
	void SetupShader();

private:
};
