#pragma once

#include <Graphics/Material.h>
#include <Math/Matrix.h>
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

	void SetShader(Shader* shader);

	void SetWorldMatrix(const sm::Matrix* worldMatrix);
	void SetPolygonMode(PolygonMode polygonMode);
	void SetGlowMultiplier(float glowMultiplier);

	bool IsOpacity() const;

	virtual void SetupRenderState();
	virtual void SetupShader();

protected:
	const sm::Matrix* m_worldMatrix;
	uint32_t m_polygonMode;
	float m_glowMultiplier;
};
