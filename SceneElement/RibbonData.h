#pragma once

#include <string>
#include <vector>

namespace SceneElement
{
	class Source;
	class Destination;
	class Path;

	class RibbonData
	{
	public:
		RibbonData();

		Source* Source;
		Destination* Destination;
		Path* Path;
	};
}
