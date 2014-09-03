#pragma once

#include <string>

class Ribbon;
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
	class StaticData;
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

	static Ribbon* CreateRibbonFromData(const std::string& sceneName, SceneElement::RibbonData* ribbon);
	static Static* CreateStaticFromData(const std::string& sceneName, SceneElement::StaticData* staticData);
};
