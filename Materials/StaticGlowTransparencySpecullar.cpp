#include "StaticGlowTransparencySpecullar.h"
#include "../DrawingRoutines.h"
#include <Graphics/Shader.h>

StaticGlowTransparencySpecullar::StaticGlowTransparencySpecullar(Shader* shader) :
	BaseGlowTransparencySpecullar(shader)
{
}

void StaticGlowTransparencySpecullar::SetupRenderState()
{
	this->BaseGlowTransparencySpecullar::SetupRenderState();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);	
}
