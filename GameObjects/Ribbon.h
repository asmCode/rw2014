#pragma once

#include "../SceneElement/RibbonData.h"
#include "../GameObject.h"
#include <string>

class TrianglesRibbon;
class StaticTriangledMesh;
class Renderable;

class Ribbon : public GameObject
{
public:
	Ribbon(const std::string& sceneName, SceneElement::RibbonData* ribbonData);
	~Ribbon();

	void Update(float time, float seconds);

private:
	TrianglesRibbon* m_decomposeAndFly;
	TrianglesRibbon* m_composeFromRibbon;

	StaticTriangledMesh* m_staticSource;
	StaticTriangledMesh* m_staticDestination;

	Renderable* m_decomposeAndFlyRenderable;
	Renderable* m_composeFromRibbonRenderable;
	Renderable* m_staticSourceRenderable;
	Renderable* m_staticDestinationRenderable;

	float m_startDecomposeTime;
	float m_finishComposeTime;
};
