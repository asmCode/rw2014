#pragma once

#include "BaseScene.h"

class Test1Scene : public BaseScene
{
public:
	bool Initialize();

	bool Update(float time, float deltaTime);
	bool Draw(float time, float deltaTime);
};

