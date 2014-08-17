#pragma once

#include <Math/Vec3.h>
#include <Graphics/MeshPart.h>

class MeshPart;

class MeshUtils
{
public:
	static void ExtractVertexPositionBuffer(sm::Vec3*& outBuffer, MeshPart* meshPart);
};
