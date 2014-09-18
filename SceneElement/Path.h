#pragma once

#include <vector>

namespace SceneElement
{
	class Key;

	class Path
	{
	public:
		std::vector<Key*> Keys;
		float Spread;
		float TriangleScale;
	};
}
