#pragma once

#include <string>

namespace SceneElement
{
	class Material;

	class StaticData
	{
	public:
		StaticData();

		std::string MeshName;
		Material* Material;
		int Order;
	};
}
