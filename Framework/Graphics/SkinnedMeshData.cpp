#include "SkinnedMeshData.h"
#include "SkinnedVertex.h"
#include "Property.h"

#include <Math\Matrix.h>

SkinnedMeshData::SkinnedMeshData() :
	id(0),
	verticesCount(0),
	vertices(NULL),
	bonesCount(0),
	bonesIds(NULL)
{
}

SkinnedMeshData::~SkinnedMeshData()
{
	if (vertices != NULL)
		delete[] vertices;
}

Property* SkinnedMeshData::FindProperty(const std::string &name)
{
	std::map<std::string, Property*>::iterator it = m_properties.find(name);
	if (it != m_properties.end())
		return it->second;

	return NULL;
}
