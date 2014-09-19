#pragma once

#include "BaseScene.h"
#include <vector>

class Animation;

class Scene07 : public BaseScene
{
public:
	bool Update(float time, float deltaTime);

private:
	Animation* m_trainAnimation;
	Animation* m_trainAnimationWire;

	void InitializeSubScene();
};
