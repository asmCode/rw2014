#pragma once

#include <string>
#include <vector>

namespace SceneElement
{
	class Source;
	class Destination;
	class StaticSource;
	class StaticDestination;
	class Path;

	class RibbonData
	{
	public:
		RibbonData();

		Source* Source;
		Destination* Destination;
		StaticSource* StaticSource;
		StaticDestination* StaticDestination;
		Path* Path;
	};
}
