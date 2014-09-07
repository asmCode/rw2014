#pragma once

#include "BaseScene.h"
#include <vector>

class GuySceneTest : public BaseScene
{
public:
	bool Update(float time, float deltaTime);

private:
	void InitializeSubScene();
};
