#pragma once

#include "../SceneElement/RibbonData.h"
#include "../GameObject.h"
#include <string>

class DecomposeToRibbon;
class ComposeFromRibbon;

class Ribbon : public GameObject
{
public:
	Ribbon(const std::string& sceneName, SceneElement::RibbonData* ribbonData);
	~Ribbon();

	void Update(float time, float seconds);

private:
	DecomposeToRibbon* m_decomposeAndFly;
	ComposeFromRibbon* m_composeFromRibbon;
};