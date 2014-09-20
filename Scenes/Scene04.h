#pragma once

#include "BaseScene.h"
#include <vector>

class GlowTransparencySpecullar;
class Shader;

class Scene04 : public BaseScene
{
public:
	bool Update(float time, float deltaTime);

private:
	Shader* m_waterShader;

	void InitializeSubScene();
};
