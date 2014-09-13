#include "Static.h"
#include "../Renderable.h"
#include "../Materials/StaticGlowTransparencySpecullar.h"
#include "../StaticTriangledMesh.h"
#include "../SceneElement/StaticData.h"
#include "../SceneElement/Material.h"
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>

Static::Static(const std::string& sceneName, SceneElement::StaticData* staticData) :
	m_mesh(NULL)
{
	Model* model = Content::Instance->Get<Model>(sceneName);
	assert(model != NULL);

	Mesh* sceneMesh = model->FindMesh(staticData->MeshName);
	assert(sceneMesh != NULL);

	m_name = staticData->MeshName;

	m_mesh = new StaticTriangledMesh();
	m_mesh->Initialize(sceneMesh->meshParts[0]);

	if (staticData->Material != NULL)
		m_mesh->SetColor(sm::Vec4(staticData->Material->Diffuse, staticData->Material->Opacity));

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("StaticSpecularBlur");
	assert(glowSpecullarShader != NULL);

	StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(glowSpecullarShader);
	material->SetGlowMultiplier(0.0f);
	StaticGlowTransparencySpecullar* materialWire = new StaticGlowTransparencySpecullar(glowSpecullarShader);
	materialWire->SetPolygonMode(StaticGlowTransparencySpecullar::PolygonMode_Lines);

	m_renderables.push_back(new Renderable(m_mesh, materialWire));
	m_renderables.push_back(new Renderable(m_mesh, material));
}

Static::~Static()
{
}

void Static::Update(float time, float seconds)
{
}

StaticTriangledMesh* Static::GetMesh() const
{
	return m_mesh;
}
