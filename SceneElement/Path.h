#pragma once

#include <vector>

namespace SceneElement
{
	class Key;
	class FloatKey;

	class Path
	{
	public:
		std::vector<Key*> Keys;
		std::vector<FloatKey*> RibbonWeights;
		float Spread;
		float TriangleScale;
		float Delay;
		bool DontRender;
	};
}
