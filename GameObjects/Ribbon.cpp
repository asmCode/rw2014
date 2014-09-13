#include "Ribbon.h"
#include "../DecomposeToRibbon.h"
#include "../ComposeFromRibbon.h"
#include "../Renderable.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../Materials/StaticGlowTransparencySpecullar.h"
#include "../UniqueTriangledMesh.h"
#include "../StaticTriangledMesh.h"
#include "../SceneElement/Path.h"
#include "../SceneElement/RibbonData.h"
#include "../SceneElement/Source.h"
#include "../SceneElement/Destination.h"
#include "../SceneElement/StaticSource.h"
#include "../SceneElement/Material.h"
#include "../SceneElement/StaticDestination.h"
#include "../SceneElement/Key.h"
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>

Ribbon::Ribbon(const std::string& sceneName, SceneElement::RibbonData* ribbonData) :
	m_decomposeAndFly(NULL),
	m_composeFromRibbon(NULL),
	m_staticSource(NULL),
	m_staticDestination(NULL),
	m_decomposeAndFlyRenderable(NULL),
	m_composeFromRibbonRenderable(NULL),
	m_staticSourceRenderable(NULL),
	m_staticDestinationRenderable(NULL),
	m_startDecomposeTime(0.0f),
	m_finishComposeTime(0.0f)
{
	assert(ribbonData != NULL);
	assert(ribbonData->Path != NULL);
	assert(ribbonData->Path->Keys.size() > 0);

	int halfPathKeyIndex = ribbonData->Path->Keys.size() / 2;

	int keysCount = ribbonData->Path->Keys.size();
	float duration = ribbonData->Path->Keys[keysCount - 1]->Time - ribbonData->Path->Keys[0]->Time;
	float spread = 5.0f;
	float minScale = 0.4f;
	float durationDelay = 4.0f;

	m_startDecomposeTime = ribbonData->Path->Keys[0]->Time;
	m_finishComposeTime = ribbonData->Path->Keys[keysCount - 1]->Time + durationDelay;

	Model* model = Content::Instance->Get<Model>(sceneName);
	assert(model != NULL);

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("SpecularBlur");
	assert(glowSpecullarShader != NULL);

	Shader* staticGlowSpecullarShader = Content::Instance->Get<Shader>("StaticSpecularBlur");
	assert(staticGlowSpecullarShader != NULL);

	Material* material = new GlowTransparencySpecullar(glowSpecullarShader);
	Material* staticMaterial = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);

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
			minScale,
			durationDelay);

		if (ribbonData->Source->Material != NULL)
			m_decomposeAndFly->GetMesh()->SetColor(sm::Vec4(ribbonData->Source->Material->Diffuse, ribbonData->Source->Material->Opacity));

		m_decomposeAndFlyRenderable = new Renderable(m_decomposeAndFly->GetMesh(), material);
		m_renderables.push_back(m_decomposeAndFlyRenderable);
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
			minScale,
			durationDelay);

		if (ribbonData->Destination->Material != NULL)
			m_composeFromRibbon->GetMesh()->SetColor(sm::Vec4(ribbonData->Destination->Material->Diffuse, ribbonData->Destination->Material->Opacity));

		m_composeFromRibbonRenderable = new Renderable(m_composeFromRibbon->GetMesh(), material);
		m_renderables.push_back(m_composeFromRibbonRenderable);
	}

	if (ribbonData->StaticSource != NULL)
	{
		m_staticSource = new StaticTriangledMesh();

		Mesh* mesh = model->FindMesh(ribbonData->StaticSource->MeshName);
		assert(mesh != NULL);

		m_staticSource->Initialize(mesh->meshParts[0]);
		if (ribbonData->StaticSource->Material != NULL)
			m_staticSource->SetColor(sm::Vec4(ribbonData->StaticSource->Material->Diffuse, ribbonData->StaticSource->Material->Opacity));

		m_staticSourceRenderable = new Renderable(m_staticSource, staticMaterial);
		m_renderables.push_back(m_staticSourceRenderable);
	}

	if (ribbonData->StaticDestination != NULL)
	{
		m_staticDestination = new StaticTriangledMesh();

		Mesh* mesh = model->FindMesh(ribbonData->StaticDestination->MeshName);
		assert(mesh != NULL);

		m_staticDestination->Initialize(mesh->meshParts[0]);
		if (ribbonData->StaticDestination->Material != NULL)
			m_staticDestination->SetColor(sm::Vec4(ribbonData->StaticDestination->Material->Diffuse, ribbonData->StaticDestination->Material->Opacity));

		m_staticDestinationRenderable = new Renderable(m_staticDestination, staticMaterial);
		m_renderables.push_back(m_staticDestinationRenderable);
	}
}

Ribbon::~Ribbon()
{
}

void Ribbon::Update(float time, float seconds)
{
	for (uint32_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->SetActive(false);

	if (time >= m_startDecomposeTime && time <= m_finishComposeTime)
	{
		m_decomposeAndFlyRenderable->SetActive(true);
		m_decomposeAndFly->Update(time, seconds);

		m_composeFromRibbonRenderable->SetActive(true);
		m_composeFromRibbon->Update(time, seconds);
	}
	else if (time < m_startDecomposeTime && m_staticSourceRenderable != NULL)
	{
		m_staticSourceRenderable->SetActive(true);
	}

	else if (time > m_finishComposeTime && m_staticDestinationRenderable != NULL)
	{
		m_staticDestinationRenderable->SetActive(true);
	}
}
