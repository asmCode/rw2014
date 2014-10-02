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
	assert(staticData != NULL);
	assert(staticData->Material != NULL);
	assert(staticData->Material->UseSolid || staticData->Material->UseWire);

	Model* model = Content::Instance->Get<Model>(sceneName);
	assert(model != NULL);

	Mesh* sceneMesh = model->FindMesh(staticData->MeshName);
	assert(sceneMesh != NULL);

	m_name = staticData->MeshName;

	m_mesh = new StaticTriangledMesh();
	m_mesh->Initialize(sceneMesh->meshParts[0]);

	m_mesh->SetGlowPower(staticData->Material->SolidGlowPower);

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("StaticSpecularBlur");
	assert(glowSpecullarShader != NULL);

	if (staticData->Material->UseSolid)
	{
		StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(glowSpecullarShader);
		material->SetGlowMultiplier(staticData->Material->SolidGlowMultiplier);
		material->SetColor(sm::Vec4(staticData->Material->Diffuse, staticData->Material->Opacity));

		Renderable* renderable = new Renderable(m_mesh, material, staticData->Order);
		m_renderables.push_back(renderable);
	}

	if (staticData->Material->UseWire)
	{
		StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(glowSpecullarShader);
		material->SetGlowMultiplier(staticData->Material->WireGlowMultiplier);
		material->SetColor(sm::Vec4(staticData->Material->Diffuse, staticData->Material->Opacity));
		material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

		Renderable* renderable = new Renderable(m_mesh, material, staticData->Order);
		m_renderables.push_back(renderable);
	}
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
