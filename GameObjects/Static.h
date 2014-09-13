#pragma once

#include "../SceneElement/StaticData.h"
#include "../GameObject.h"
#include <string>

class StaticTriangledMesh;

class Static : public GameObject
{
public:
	Static(const std::string& sceneName, SceneElement::StaticData* staticData);
	~Static();

	void Update(float time, float seconds);

	StaticTriangledMesh* GetMesh() const;

private:
	StaticTriangledMesh* m_mesh;
};
