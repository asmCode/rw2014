#pragma once

#include <Graphics/Material.h>
#include <stdint.h>

class StaticGlowTransparencySpecullar : public Material
{
public:
	StaticGlowTransparencySpecullar(Shader* shader);

	bool IsOpacity() const;
	void SetupRenderState();
	void SetupShader();

private:
};
