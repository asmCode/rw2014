#pragma once

#include "BaseScene.h"
#include <vector>

class CamsTestScene : public BaseScene
{
public:
	bool Update(float time, float deltaTime);

	ICamera* GetCamera() const;

private:
	void InitializeSubScene();
};
