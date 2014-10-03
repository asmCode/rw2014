#pragma once

#include <Math\Vec3.h>
#include <Math\Vec4.h>
#include <string>

namespace SceneElement
{
	class Material
	{
	public:
		std::string Name;
		sm::Vec3 Diffuse;
		float Opacity;
		bool UseSolid;
		bool UseWire;
		float SolidGlowPower;
		float SolidGlowMultiplier;
		float WireGlowPower;
		float WireGlowMultiplier;
	};
}
