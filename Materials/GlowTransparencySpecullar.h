#pragma once

#include <Graphics/Material.h>
#include <stdint.h>

class GlowTransparencySpecullar : public Material
{
public:
	enum PolygonMode
	{
		PolygonMode_Lines,
		PolygonMode_Fill
	};

	GlowTransparencySpecullar(Shader* shader);

	void SetPolygonMode(PolygonMode polygonMode);

	bool IsOpacity() const;
	void SetupRenderState();
	void SetupShader();

private:
	uint32_t m_polygonMode;
};
