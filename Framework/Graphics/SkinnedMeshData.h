#pragma once

#include <Math\Matrix.h>

#include <vector>
#include <map>
#include <string>

class Property;
class SkinnedVertex;

class SkinnedMeshData
{
	friend class SkinnedMeshLoader;

public:
	int id;
	std::string name;
	std::string materialName;

	SkinnedMeshData();
	~SkinnedMeshData();

	Property* FindProperty(const std::string &name);

	void Initialize();

	sm::Matrix m_worldInverseMatrix;
	sm::Matrix m_worldMatrix;

public:
	int verticesCount;
	SkinnedVertex *vertices;

	int bonesCount;
	int* bonesIds;

	std::map<std::string, Property*> m_properties;
};
