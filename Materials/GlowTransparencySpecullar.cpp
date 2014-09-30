#include "GlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include <Graphics/Shader.h>

GlowTransparencySpecullar::GlowTransparencySpecullar(Shader* shader) :
	BaseGlowTransparencySpecullar(shader)
{
}

void GlowTransparencySpecullar::SetupRenderState()
{
	this->BaseGlowTransparencySpecullar::SetupRenderState();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(7);
	glDisableVertexAttribArray(8);
}
