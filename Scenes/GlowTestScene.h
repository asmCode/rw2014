#pragma once

#include "BaseScene.h"

class GlowTestScene : public BaseScene
{
public:
	GlowTestScene();
	virtual ~GlowTestScene();

	bool Initialize();

	bool Update(float time, float deltaTime);

private:
	GameObject* m_teapot;
};
