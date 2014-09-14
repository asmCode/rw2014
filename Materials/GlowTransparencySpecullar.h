#pragma once

#include "BaseGlowTransparencySpecullar.h"
#include <stdint.h>

class GlowTransparencySpecullar : public BaseGlowTransparencySpecullar
{
public:
	GlowTransparencySpecullar(Shader* shader);

	void SetupRenderState();
};
