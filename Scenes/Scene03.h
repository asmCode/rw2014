#pragma once

#include "BaseScene.h"
#include <vector>

class Scene03 : public BaseScene
{
public:
	bool Update(float time, float deltaTime);

private:
	void InitializeSubScene();
};
