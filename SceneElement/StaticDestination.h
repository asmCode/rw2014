#pragma once

#include <string>

namespace SceneElement
{
	class Material;

	class StaticDestination
	{
	public:
		std::string MeshName;
		Material* Material;
	};
}
