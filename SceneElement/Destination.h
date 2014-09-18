#pragma once

#include <string>

namespace SceneElement
{
	class Material;

	class Destination
	{
	public:
		std::string MeshName;
		Material* Material;
		bool Stay;
	};
}
