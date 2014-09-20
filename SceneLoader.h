#pragma once

#include <string>
#include <vector>

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
	static bool LoadFromFile(BaseScene* scene, const std::string& sceneName);

private:
	static SceneElement::RibbonData* LoadRibbon(XMLNode* node);
	static SceneElement::Source* LoadSource(XMLNode* node);
	static SceneElement::Destination* LoadDestination(XMLNode* node);
	static SceneElement::StaticSource* LoadStaticSource(XMLNode* node);
	static SceneElement::StaticDestination* LoadStaticDestination(XMLNode* node);
	static SceneElement::Path* LoadPath(XMLNode* node);
	static SceneElement::Key* LoadKey(XMLNode* node);
	static SceneElement::StaticData* LoadStatic(XMLNode* node);
	static SceneElement::GuyData* LoadGuy(XMLNode* node);
	static SceneElement::Material* LoadMaterial(XMLNode* materialNode);
	static SceneElement::Material* LoadMaterialFromChild(XMLNode* node);
	static void LoadIntKeys(XMLNode* node, std::vector<SceneElement::IntKey*>& keys);
	static void LoadFloatKeys(XMLNode* node, std::vector<SceneElement::FloatKey*>& keys);

	static Ribbon* CreateRibbonFromData(const std::string& sceneName, SceneElement::RibbonData* ribbon);
	static Static* CreateStaticFromData(const std::string& sceneName, SceneElement::StaticData* staticData);
	static Guy* CreateGuyFromData(const std::string& sceneName, SceneElement::GuyData* guyData);
};
