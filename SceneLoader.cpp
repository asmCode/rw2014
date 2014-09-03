#include "SceneLoader.h"
#include "Environment.h"
#include "GameObject.h"
#include "DemoUtils.h"
#include "GameObjects/Ribbon.h"
#include "GameObjects/Static.h"
#include "Scenes/BaseScene.h"

#include "SceneElement/RibbonData.h"
#include "SceneElement/Source.h"
#include "SceneElement/Destination.h"
#include "SceneElement/StaticSource.h"
#include "SceneElement/StaticDestination.h"
#include "SceneElement/Path.h"
#include "SceneElement/Key.h"
#include "SceneElement/StaticData.h"

#include <XML/XMLNode.h>
#include <XML/XMLLoader.h>

bool SceneLoader::LoadFromFile(BaseScene* scene, const std::string& sceneName)
{
	std::string path = Environment::GetInstance()->GetBasePath() + "Scenes\\" + sceneName + ".scene";

	XMLNode* node = XMLLoader::LoadFromFile(path);
	if (node == NULL)
		return false;

	scene->m_name = node->GetAttribAsString("name");

	XMLNode* ribbonsNode = node->GetChild("Ribbons");
	if (ribbonsNode == NULL)
		return false;

	for (uint32_t i = 0; i < ribbonsNode->GetChildrenCount(); i++)
	{
		SceneElement::RibbonData* ribbonData = LoadRibbon(ribbonsNode->GetChild(i));
		if (ribbonData == NULL)
			continue;

		Ribbon* ribbon = CreateRibbonFromData(scene->m_name, ribbonData);
		if (ribbon != NULL)
			scene->m_gameObjects.push_back(ribbon);
	}

	XMLNode* staticMeshesNode = node->GetChild("StaticMeshes");
	if (staticMeshesNode != NULL)
	{
		for (uint32_t i = 0; i < staticMeshesNode->GetChildrenCount(); i++)
		{
			SceneElement::StaticData* staticData = LoadStatic(staticMeshesNode->GetChild(i));
			if (staticData == NULL)
				continue;

			Static* staticMesh = CreateStaticFromData(scene->m_name, staticData);
			if (staticMesh != NULL)
				scene->m_gameObjects.push_back(staticMesh);
		}
	}

	return true;
}

SceneElement::RibbonData* SceneLoader::LoadRibbon(XMLNode* node)
{
	assert(node->GetName() == "Ribbon");

	SceneElement::RibbonData* ribbon = new SceneElement::RibbonData();

	XMLNode* sourceNode = node->GetChild("Source");
	if (sourceNode != NULL)
		ribbon->Source = LoadSource(sourceNode);

	XMLNode* destinationNode = node->GetChild("Destination");
	if (destinationNode != NULL)
		ribbon->Destination = LoadDestination(destinationNode);

	XMLNode* staticSourceNode = node->GetChild("StaticSource");
	if (staticSourceNode != NULL)
		ribbon->StaticSource = LoadStaticSource(staticSourceNode);

	XMLNode* staticDestinationNode = node->GetChild("StaticDestination");
	if (staticDestinationNode != NULL)
		ribbon->StaticDestination = LoadStaticDestination(staticDestinationNode);

	XMLNode* pathNode = node->GetChild("Path");
	if (pathNode != NULL)
		ribbon->Path = LoadPath(pathNode);

	return ribbon;
}

SceneElement::Source* SceneLoader::LoadSource(XMLNode* node)
{
	SceneElement::Source* source = new SceneElement::Source();
	source->MeshName = node->GetAttribAsString("mesh_name");
	return source;
}

SceneElement::Destination* SceneLoader::LoadDestination(XMLNode* node)
{
	SceneElement::Destination* destination = new SceneElement::Destination();
	destination->MeshName = node->GetAttribAsString("mesh_name");
	return destination;
}

SceneElement::StaticSource* SceneLoader::LoadStaticSource(XMLNode* node)
{
	SceneElement::StaticSource* source = new SceneElement::StaticSource();
	source->MeshName = node->GetAttribAsString("mesh_name");
	return source;
}

SceneElement::StaticDestination* SceneLoader::LoadStaticDestination(XMLNode* node)
{
	SceneElement::StaticDestination* destination = new SceneElement::StaticDestination();
	destination->MeshName = node->GetAttribAsString("mesh_name");
	return destination;
}

SceneElement::Path* SceneLoader::LoadPath(XMLNode* node)
{
	SceneElement::Path* path = new SceneElement::Path();

	for (uint32_t i = 0; i < node->GetChildrenCount(); i++)
	{
		SceneElement::Key *key = LoadKey(node->GetChild(i));
		if (key != NULL)
			path->Keys.push_back(key);
	}

	return path;
}

SceneElement::Key* SceneLoader::LoadKey(XMLNode* node)
{
	SceneElement::Key* key = new SceneElement::Key();

	key->Time = node->GetAttribAsFloat("time");
	key->Position = DemoUtils::ParseVector3(node->GetAttribAsString("position"));
	key->Rotation.Set(0.0f, 1.0f, 0.0f, 0.0f);
	key->Scale.Set(1.0f, 1.0f, 1.0f);

	return key;
}

SceneElement::StaticData* SceneLoader::LoadStatic(XMLNode* node)
{
	SceneElement::StaticData* data = new SceneElement::StaticData();
	data->MeshName = node->GetAttribAsString("mesh_name");
	return data;
}

Ribbon* SceneLoader::CreateRibbonFromData(const std::string& sceneName, SceneElement::RibbonData* ribbon)
{
	return new Ribbon(sceneName, ribbon);
}

Static* SceneLoader::CreateStaticFromData(const std::string& sceneName, SceneElement::StaticData* staticData)
{
	return new Static(sceneName, staticData);
}
