#pragma once

#include <Graphics/Material.h>
#include <stdint.h>

class BaseGlowTransparencySpecullar : public Material
{
public:
	enum PolygonMode
	{
		PolygonMode_Lines,
		PolygonMode_Fill
	};

	BaseGlowTransparencySpecullar(Shader* shader);

	void SetPolygonMode(PolygonMode polygonMode);
	void SetGlowMultiplier(float glowMultiplier);

	bool IsOpacity() const;

	virtual void SetupRenderState();
	virtual void SetupShader();

protected:
	uint32_t m_polygonMode;
	float m_glowMultiplier;
};
