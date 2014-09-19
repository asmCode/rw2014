#pragma once

#include "BaseScene.h"
#include <vector>

class Scene03 : public BaseScene
{
public:
	bool Update(float time, float deltaTime);

	float GetStartTime() const;
	float GetEndTime() const;

private:
	void InitializeSubScene();
};
