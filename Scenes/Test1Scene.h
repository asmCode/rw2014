#pragma once

#include "BaseScene.h"

class BumpAlongNormal;
class DecomposeToRibbon;
class ComposeFromRibbon;

class Test1Scene : public BaseScene
{
public:
	bool Initialize();

	bool Update(float time, float deltaTime);
	bool Draw(float time, float deltaTime);

private:
	BumpAlongNormal* m_triangleShader;
	DecomposeToRibbon* m_decomposeAndFly;
	ComposeFromRibbon* m_composeFromRibbon;
};

