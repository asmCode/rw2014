#include "SkinnedMeshLoader.h"
#include "SkinnedMeshData.h"
#include "SkinnedVertex.h"
#include "Property.h"

#include <IO\BinaryReader.h>
#include <IO\Path.h>
#include <Utils/Log.h>

#include <sstream>

SkinnedMeshData* SkinnedMeshLoader::LoadFromFile(const std::string &path)
{
	uint8_t *buff;
	uint32_t fileSize;

	if (!Path::GetFileContent(path.c_str(), buff, fileSize))
	{
		assert(false && "couldn't load model file");
		return NULL;
	}

	BinaryReader br(buff);

	if (fileSize >= 8)
	{
		BinaryReader *sigBr = new BinaryReader(buff);

		char fileSignature[6];
		sigBr->ReadBuffer(fileSignature, 6);
		unsigned short fileVersion = sigBr->Read<unsigned short>();

		if (memcmp("FTSMDL", fileSignature, 6) == 0)
		{
			// skip 8 bytes
			br.Read<int>();
			br.Read<int>();
		}
	}

	int meshesCount = br.Read<int>();
	//assert(meshesCount == 1);

	SkinnedMeshData *mesh = LoadMesh(br);

	delete [] buff;

	return mesh;
}

SkinnedMeshData* SkinnedMeshLoader::LoadMesh(BinaryReader &br)
{
	SkinnedMeshData* mesh = new SkinnedMeshData();

	mesh ->id = br.Read<int>();
	mesh->name = br.Read<std::string>();
	mesh->materialName = br.Read<std::string>();

	for (int i = 0; i < 16; i++)
		mesh->m_worldInverseMatrix.a[i] = br.Read<float>();

	mesh->m_worldMatrix = mesh->m_worldInverseMatrix.GetInversed();

	mesh->bonesCount = br.Read<int>();
	mesh->bonesIds = new int[mesh->bonesCount];
	br.ReadBuffer((char*)mesh->bonesIds, sizeof(int)* mesh->bonesCount);

	mesh->verticesCount = br.Read<int>();
	mesh->vertices = new SkinnedVertex[mesh->verticesCount];
	br.ReadBuffer((char*)mesh->vertices, sizeof(SkinnedVertex) * mesh->verticesCount);

	int propsCount = br.Read<int>();
	for (int i = 0; i < propsCount; i++)
	{
		Property *prop = LoadProperty(br);
		mesh->m_properties[prop->GetName()] = prop;
	}

	return mesh;
}

Property* SkinnedMeshLoader::LoadProperty(BinaryReader &br)
{
	std::string name = br.Read<std::string>();
	Property::PropertyType type = (Property::PropertyType)br.Read<BYTE>();
	Property::AnimationType animType = (Property::AnimationType)br.Read<BYTE>();

	Property *prop = new Property(name, type, animType);

	if (animType == Property::AnimationType_None)
	{
		switch (type)
		{
		case Property::PropertyType_Boolean:
			prop->SetValue(br.Read<bool>());
			break;

		case Property::PropertyType_Int:
			prop->SetValue(br.Read<int>());
			break;

		case Property::PropertyType_Float:
			prop->SetValue(br.Read<float>());
			break;

		case Property::PropertyType_Vector3:
			{
				sm::Vec3 val;
				val.x = br.Read<float>();
				val.y = br.Read<float>();
				val.z = br.Read<float>();
				prop->SetValue(val);
			}
			break;

		case Property::PropertyType_String:
			prop->SetValue(br.Read<std::string>());
			break;
		}
	}
	else
	{
		int keysCount = br.Read<int>();

		for (int i = 0; i < keysCount; i++)
		{
			float time = br.Read<float>();

			switch (type)
			{
			case Property::PropertyType_Float:
				{
					float value = br.Read<float>();
					prop->SetValue(value, time);
				}
				break;

			case Property::PropertyType_Int:
				{
					int value = br.Read<int>();
					prop->SetValue(value, time);
				}
				break;
			}
		}
	}

	return prop;
}

