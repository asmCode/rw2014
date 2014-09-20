#pragma once

#include "../SceneElement/RibbonData.h"
#include "../GameObject.h"
#include <Math/Animation/AnimationCurve.h>
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

	TrianglesRibbon* GetSource();

	AnimationCurve<float>* m_ribbonWeightCurve;

private:
	TrianglesRibbon* m_decomposeAndFly;
	TrianglesRibbon* m_composeFromRibbon;

	StaticTriangledMesh* m_staticSource;
	StaticTriangledMesh* m_staticDestination;

	Renderable* m_decomposeAndFlyRenderable;
	Renderable* m_decomposeAndFlyRenderableWire;
	Renderable* m_composeFromRibbonRenderable;
	Renderable* m_composeFromRibbonRenderableWire;
	Renderable* m_staticSourceRenderable;
	Renderable* m_staticSourceRenderableWire;
	Renderable* m_staticDestinationRenderable;
	Renderable* m_staticDestinationRenderableWire;

	float m_startDecomposeTime;
	float m_finishComposeTime;
};
