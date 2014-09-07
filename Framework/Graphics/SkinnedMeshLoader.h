#pragma once

#include <string>

class SkinnedMeshData;
class SkinnedVertex;
class Property;
class BinaryReader;

class SkinnedMeshLoader
{
public:
	static SkinnedMeshData* LoadFromFile(const std::string &path);

private:
	static SkinnedMeshData* LoadMesh(BinaryReader &br);
	static Property *LoadProperty(BinaryReader &br);
};

