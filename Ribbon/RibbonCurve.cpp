#include "RibbonCurve.h"
/*
#include "TriangledMesh.h"
#include "UniqueTriangledMesh.h"
#include "Triangle.h"
#include "DemoUtils.h"
#include "DebugUtils.h"
#include "SceneElement/Path.h"
#include "SceneElement/Key.h"
#include <Math/Animation/QuarticOut.h>
#include <Utils/Randomizer.h>
#include <Graphics/Interpolators/TCBInterpolator.h>
#include <Math/Animation/AnimationCurve.h>
#include <Math/Animation/QuadOut.h>
#include <Math/Animation/LinearCurve.h>
#include <Math/Animation/Custom/BlinkCurve.h>
*/

AnimationCurve<sm::Vec3>* RibbonCurve::CreateCurve(
	const sm::Vec3& basePosition,
	const sm::Vec3& normal,
	SceneElement::Path* path,
	int endKeyIndex,
	float spread,
	float maxDelay)
{
	return NULL;
}

AnimationCurve<float>* RibbonCurve::CreateScaleCurve(AnimationCurve<sm::Vec3> *transformCurve, float minScale)
{
	return NULL;
}
