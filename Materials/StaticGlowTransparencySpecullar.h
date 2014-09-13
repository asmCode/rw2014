#pragma once

#include "BaseGlowTransparencySpecullar.h"
#include <stdint.h>

class StaticGlowTransparencySpecullar : public BaseGlowTransparencySpecullar
{
public:
	StaticGlowTransparencySpecullar(Shader* shader);

	void SetupRenderState();
};
