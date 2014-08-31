#pragma once

#include <string>

class Ribbon;
class BaseScene;

namespace SceneElement
{
	class RibbonData;
	class Source;
	class Destination;
	class Path;
	class Key;
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
	static SceneElement::Path* LoadPath(XMLNode* node);
	static SceneElement::Key* LoadKey(XMLNode* node);

	static Ribbon* CreateRibbonFromData(const std::string& sceneName, SceneElement::RibbonData* ribbon);
};
