#pragma once

#include "BaseScene.h"
#include <vector>

class BumpAlongNormal;
class DecomposeToRibbon;
class ComposeFromRibbon;
class GameObject;

class Test1Scene : public BaseScene
{
public:
	bool Initialize();

	bool Update(float time, float deltaTime);

private:
	BumpAlongNormal* m_triangleShader;
	DecomposeToRibbon* m_decomposeAndFly;
	ComposeFromRibbon* m_composeFromRibbon;
};

