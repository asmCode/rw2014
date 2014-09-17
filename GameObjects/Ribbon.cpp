#include "Ribbon.h"
#include "../Renderable.h"
#include "../Materials/GlowTransparencySpecullar.h"
#include "../Materials/StaticGlowTransparencySpecullar.h"
#include "../UniqueTriangledMesh.h"
#include "../StaticTriangledMesh.h"
#include "../TrianglesRibbon.h"
#include "../SceneElement/Path.h"
#include "../SceneElement/RibbonData.h"
#include "../SceneElement/Source.h"
#include "../SceneElement/Destination.h"
#include "../SceneElement/StaticSource.h"
#include "../SceneElement/Material.h"
#include "../SceneElement/StaticDestination.h"
#include "../SceneElement/Key.h"
#include "../Ribbon/RibbonCurveFullSource.h"
#include "../Ribbon/RibbonCurveFullDestination.h"
#include "../Ribbon/RibbonCurveDestination.h"
#include "../Ribbon/RibbonCurveSource.h"
#include "../Ribbon/Modificators/BlinkAtStartAndEnd.h"
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
	m_decomposeAndFlyRenderableWire(NULL),
	m_composeFromRibbonRenderable(NULL),
	m_composeFromRibbonRenderableWire(NULL),
	m_staticSourceRenderable(NULL),
	m_staticSourceRenderableWire(NULL),
	m_staticDestinationRenderable(NULL),
	m_staticDestinationRenderableWire(NULL),
	m_startDecomposeTime(0.0f),
	m_finishComposeTime(0.0f)
{
	assert(ribbonData != NULL);
	assert(ribbonData->Path != NULL);
	assert(ribbonData->Path->Keys.size() > 0);

	int halfPathKeyIndex = ribbonData->Path->Keys.size() / 2;

	int keysCount = ribbonData->Path->Keys.size();
	float duration = ribbonData->Path->Keys[keysCount - 1]->Time - ribbonData->Path->Keys[0]->Time;
	float spread = 2.0f;
	float minScale = 0.6f;
	float durationDelay = 4.0f;

	m_startDecomposeTime = ribbonData->Path->Keys[0]->Time;
	m_finishComposeTime = ribbonData->Path->Keys[keysCount - 1]->Time + durationDelay;

	Model* model = Content::Instance->Get<Model>(sceneName);
	assert(model != NULL);

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("SpecularBlur");
	assert(glowSpecullarShader != NULL);

	Shader* staticGlowSpecullarShader = Content::Instance->Get<Shader>("StaticSpecularBlur");
	assert(staticGlowSpecullarShader != NULL);

	/*
	StaticGlowTransparencySpecullar* staticMaterial = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
	staticMaterial->SetGlowMultiplier(0.8f);
	StaticGlowTransparencySpecullar* staticMaterialWire = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
	staticMaterialWire->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);
	staticMaterialWire->SetGlowMultiplier(1.2f);
	*/

	IRibbonCurveBuilder* sourceCurveBuilder = NULL;
	IRibbonCurveBuilder* destinationCurveBuilder = NULL;

	if (ribbonData->Source != NULL && ribbonData->Destination == NULL)
		sourceCurveBuilder = new RibbonCurveSource();
	else if (ribbonData->Source == NULL && ribbonData->Destination != NULL)
		destinationCurveBuilder = new RibbonCurveDestination();
	else if (ribbonData->Source != NULL && ribbonData->Destination != NULL)
	{
		sourceCurveBuilder = new RibbonCurveFullSource();
		destinationCurveBuilder = new RibbonCurveFullDestination();
	}

	if (ribbonData->Source != NULL)
	{
		assert(ribbonData->Source->Material != NULL);
		assert(ribbonData->Source->Material->UseSolid || ribbonData->Source->Material->UseWire);

		m_decomposeAndFly = new TrianglesRibbon();

		Mesh* mesh = model->FindMesh(ribbonData->Source->MeshName);
		assert(mesh != NULL);

		m_decomposeAndFly->Initialize(
			sourceCurveBuilder,
			mesh->meshParts[0],
			ribbonData->Path,
			halfPathKeyIndex,
			spread,
			minScale,
			durationDelay);

		m_decomposeAndFly->SetTriangleModificator(BlinkAtStartAndEnd::GetInstance());

		m_decomposeAndFly->GetMesh()->SetGlowPower(ribbonData->Source->Material->SolidGlowPower); // wspolne dla solid i wire
		m_decomposeAndFly->GetMesh()->SetColor(sm::Vec4(ribbonData->Source->Material->Diffuse, ribbonData->Source->Material->Opacity));

		if (ribbonData->Source->Material->UseSolid)
		{
			GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->Source->Material->SolidGlowMultiplier);

			m_decomposeAndFlyRenderable = new Renderable(m_decomposeAndFly->GetMesh(), material);
			m_renderables.push_back(m_decomposeAndFlyRenderable);
		}

		if (ribbonData->Source->Material->UseWire)
		{
			GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->Source->Material->WireGlowMultiplier);
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			m_decomposeAndFlyRenderableWire = new Renderable(m_decomposeAndFly->GetMesh(), material);
			m_renderables.push_back(m_decomposeAndFlyRenderableWire);
		}
	}

	if (ribbonData->Destination != NULL)
	{
		assert(ribbonData->Destination->Material != NULL);
		assert(ribbonData->Destination->Material->UseSolid || ribbonData->Destination->Material->UseWire);

		m_composeFromRibbon = new TrianglesRibbon();

		Mesh* mesh = model->FindMesh(ribbonData->Destination->MeshName);
		assert(mesh != NULL);

		m_composeFromRibbon->Initialize(
			destinationCurveBuilder,
			mesh->meshParts[0],
			ribbonData->Path,
			halfPathKeyIndex,
			spread,
			minScale,
			durationDelay);

		m_composeFromRibbon->GetMesh()->SetGlowPower(ribbonData->Destination->Material->SolidGlowPower); // wspolne dla solid i wire
		m_composeFromRibbon->GetMesh()->SetColor(sm::Vec4(ribbonData->Destination->Material->Diffuse, ribbonData->Destination->Material->Opacity));

		if (ribbonData->Destination->Material->UseSolid)
		{
			GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->Destination->Material->SolidGlowMultiplier);

			m_composeFromRibbonRenderable = new Renderable(m_composeFromRibbon->GetMesh(), material);
			m_renderables.push_back(m_composeFromRibbonRenderable);
		}

		if (ribbonData->Destination->Material->UseWire)
		{
			GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->Destination->Material->WireGlowMultiplier);
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			m_composeFromRibbonRenderableWire = new Renderable(m_composeFromRibbon->GetMesh(), material);
			m_renderables.push_back(m_composeFromRibbonRenderableWire);
		}
	}

	if (ribbonData->StaticSource != NULL)
	{
		assert(ribbonData->StaticSource->Material != NULL);
		assert(ribbonData->StaticSource->Material->UseSolid || ribbonData->StaticSource->Material->UseWire);

		Mesh* mesh = model->FindMesh(ribbonData->StaticSource->MeshName);
		assert(mesh != NULL);

		m_staticSource = new StaticTriangledMesh();
		m_staticSource->Initialize(mesh->meshParts[0]);

		m_staticSource->SetGlowPower(ribbonData->StaticSource->Material->SolidGlowPower); // wspolne dla solid i wire
		m_staticSource->SetColor(sm::Vec4(ribbonData->StaticSource->Material->Diffuse, ribbonData->StaticSource->Material->Opacity)); // wspolne dla wire i solid

		if (ribbonData->StaticSource->Material->UseSolid)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->StaticSource->Material->SolidGlowMultiplier);

			m_staticSourceRenderable = new Renderable(m_staticSource, material);
			m_renderables.push_back(m_staticSourceRenderable);
		}

		if (ribbonData->StaticSource->Material->UseWire)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->StaticSource->Material->WireGlowMultiplier);
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			m_staticSourceRenderableWire = new Renderable(m_staticSource, material);
			m_renderables.push_back(m_staticSourceRenderableWire);
		}
	}

	if (ribbonData->StaticDestination != NULL)
	{
		assert(ribbonData->StaticDestination->Material != NULL);
		assert(ribbonData->StaticDestination->Material->UseSolid || ribbonData->StaticDestination->Material->UseWire);

		Mesh* mesh = model->FindMesh(ribbonData->StaticDestination->MeshName);
		assert(mesh != NULL);

		m_staticDestination = new StaticTriangledMesh();
		m_staticDestination->Initialize(mesh->meshParts[0]);

		m_staticDestination->SetGlowPower(ribbonData->StaticDestination->Material->SolidGlowPower); // wspolne dla solid i wire
		m_staticDestination->SetColor(sm::Vec4(ribbonData->StaticDestination->Material->Diffuse, ribbonData->StaticDestination->Material->Opacity)); // wspolne dla wire i solid

		if (ribbonData->StaticDestination->Material->UseSolid)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->StaticDestination->Material->SolidGlowMultiplier);

			m_staticDestinationRenderable = new Renderable(m_staticDestination, material);
			m_renderables.push_back(m_staticDestinationRenderable);
		}

		if (ribbonData->StaticDestination->Material->UseWire)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(ribbonData->StaticDestination->Material->WireGlowMultiplier);
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			m_staticDestinationRenderableWire = new Renderable(m_staticDestination, material);
			m_renderables.push_back(m_staticDestinationRenderableWire);
		}
	}
}

Ribbon::~Ribbon()
{
}

void Ribbon::Update(float time, float seconds)
{
	for (uint32_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->SetActive(false);

	if (time >= m_startDecomposeTime && time <= m_finishComposeTime + 1.0f)
	{
		if (m_decomposeAndFlyRenderable != NULL)
			m_decomposeAndFlyRenderable->SetActive(true);
		if (m_decomposeAndFlyRenderableWire != NULL)
			m_decomposeAndFlyRenderableWire->SetActive(true);
		if (m_decomposeAndFly!= NULL)
			m_decomposeAndFly->Update(time, seconds);

		if (m_composeFromRibbonRenderable != NULL)
			m_composeFromRibbonRenderable->SetActive(true);
		if (m_composeFromRibbonRenderableWire != NULL)
			m_composeFromRibbonRenderableWire->SetActive(true);
		if (m_composeFromRibbon != NULL)
			m_composeFromRibbon->Update(time, seconds);
	}
	else if (time < m_startDecomposeTime && m_staticSourceRenderable != NULL)
	{
		m_staticSourceRenderable->SetActive(true);
		m_staticSourceRenderableWire->SetActive(true);
	}

	else if (time > m_finishComposeTime && m_staticDestinationRenderable != NULL)
	{
		m_staticDestinationRenderable->SetActive(true);
		m_staticDestinationRenderableWire->SetActive(true);
	}
}
