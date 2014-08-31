#include "Ribbon.h"
#include "../DecomposeToRibbon.h"
#include "../ComposeFromRibbon.h"
#include "../Renderable.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../UniqueTriangledMesh.h"
#include "../SceneElement/Path.h"
#include "../SceneElement/Source.h"
#include "../SceneElement/Destination.h"
#include "../SceneElement/Key.h"
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>

Ribbon::Ribbon(const std::string& sceneName, SceneElement::RibbonData* ribbonData) :
	m_decomposeAndFly(NULL),
	m_composeFromRibbon(NULL)
{
	int halfPathKeyIndex = ribbonData->Path->Keys.size() / 2;

	float spread = 1.0f;
	float minScale = 0.4f;

	Model* model = Content::Instance->Get<Model>(sceneName);
	assert(model != NULL);

	if (ribbonData->Source != NULL)
	{
		m_decomposeAndFly = new DecomposeToRibbon();

		Mesh* mesh = model->FindMesh(ribbonData->Source->MeshName);
		assert(mesh != NULL);

		m_decomposeAndFly->Initialize(
			mesh->meshParts[0],
			ribbonData->Path,
			halfPathKeyIndex,
			spread,
			minScale);
	}

	if (ribbonData->Destination != NULL)
	{
		m_composeFromRibbon = new ComposeFromRibbon();

		Mesh* mesh = model->FindMesh(ribbonData->Destination->MeshName);
		assert(mesh != NULL);

		m_composeFromRibbon->Initialize(
			mesh->meshParts[0],
			ribbonData->Path,
			halfPathKeyIndex,
			spread,
			minScale);
	}

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("SpecularBlur");
	assert(glowSpecullarShader != NULL);

	Material* material = new GlowTransparencySpecullar(glowSpecullarShader);

	m_renderables.push_back(new Renderable(m_decomposeAndFly->GetMesh(), material));
	m_renderables.push_back(new Renderable(m_composeFromRibbon->GetMesh(), material));
}

Ribbon::~Ribbon()
{
}

void Ribbon::Update(float time, float seconds)
{
	m_decomposeAndFly->Update(time, seconds);
	m_composeFromRibbon->Update(time, seconds);
}
