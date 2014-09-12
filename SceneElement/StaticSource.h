#pragma once

#include <string>

namespace SceneElement
{
	class Material;

	class StaticSource
	{
	public:
		std::string MeshName;
		Material* Material;
	};
}
