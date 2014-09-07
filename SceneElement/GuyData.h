#pragma once

#include <string>
#include <vector>

namespace SceneElement
{
	class Path;
	class IntKey;

	class GuyData
	{
	public:
		GuyData();

		std::string Id;
		Path* Path;
		std::vector<IntKey*> AnimationIndex;
	};
}
