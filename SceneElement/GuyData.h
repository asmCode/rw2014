#pragma once

#include <string>
#include <vector>

namespace SceneElement
{
	class Path;
	class IntKey;
	class Material;

	class GuyData
	{
	public:
		GuyData();

		std::string Id;
		Material* Material;
		Path* Path;
		std::vector<IntKey*> AnimationIndex;
		std::string RibbonName;
	};
}
