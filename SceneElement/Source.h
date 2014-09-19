#pragma once

#include <string>

namespace SceneElement
{
	class Material;

	class Source
	{
	public:
		std::string MeshName;
		Material* Material;
		bool Stay;
		bool Destroy;
	};
}
