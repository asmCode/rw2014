#pragma once

#include "BaseScene.h"
#include <vector>

class Test1Scene : public BaseScene
{
public:
	bool Update(float time, float deltaTime);

private:
	void InitializeSubScene();
};
