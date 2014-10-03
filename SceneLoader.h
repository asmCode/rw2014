#pragma once

#include <string>
#include <vector>
#include <map>

class Ribbon;
class Guy;
class Static;
class BaseScene;

namespace SceneElement
{
	class RibbonData;
	class Source;
	class Destination;
	class StaticSource;
	class StaticDestination;
	class Path;
	class Key;
	class IntKey;
	class FloatKey;
	class StaticData;
	class GuyData;
	class Material;
}

class XMLNode;

class SceneLoader
{
public:
	bool LoadFromFile(BaseScene* scene, const std::string& sceneName);

private:
	std::map<std::string, SceneElement::Material*> m_materials;

	SceneElement::RibbonData* LoadRibbon(XMLNode* node);
	SceneElement::Source* LoadSource(XMLNode* node);
	SceneElement::Destination* LoadDestination(XMLNode* node);
	SceneElement::StaticSource* LoadStaticSource(XMLNode* node);
	SceneElement::StaticDestination* LoadStaticDestination(XMLNode* node);
	SceneElement::Path* LoadPath(XMLNode* node);
	SceneElement::Key* LoadKey(XMLNode* node);
	SceneElement::StaticData* LoadStatic(XMLNode* node);
	SceneElement::GuyData* LoadGuy(XMLNode* node);
	SceneElement::Material* LoadMaterial(XMLNode* materialNode);
	static void LoadIntKeys(XMLNode* node, std::vector<SceneElement::IntKey*>& keys);
	static void LoadFloatKeys(XMLNode* node, std::vector<SceneElement::FloatKey*>& keys);

	Ribbon* CreateRibbonFromData(const std::string& sceneName, SceneElement::RibbonData* ribbon);
	Static* CreateStaticFromData(const std::string& sceneName, SceneElement::StaticData* staticData);
	Guy* CreateGuyFromData(const std::string& sceneName, SceneElement::GuyData* guyData);

	SceneElement::Material* FindMaterial(const std::string& materialName);
};
