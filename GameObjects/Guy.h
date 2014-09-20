#pragma once

#include "../SceneElement/GuyData.h"
#include "../GameObject.h"
#include <Math/Vec3.h>
#include <Graphics/Interpolators/StateInterpolator.h>
#include <string>
#include <vector>

class Renderable;
class Animation;
class SkinnedMesh;
class SkinnedRibbonMesh;
class Ribbon;
template <typename T> class AnimationCurve;

class Guy : public GameObject
{
public:
	Guy(const std::string& sceneName, SceneElement::GuyData* guyData);
	~Guy();

	void Update(float time, float seconds);
	void SetRibbon(Ribbon* ribbon);

	std::string m_ribbonName;

private:
	AnimationCurve<sm::Vec3>* m_positionCurve;
	SkinnedMesh* m_mesh;
	SkinnedRibbonMesh* m_meshRibbon;

	std::vector<Animation*> m_animations;
	StateInterpolator<int> m_animationIndex;
};
