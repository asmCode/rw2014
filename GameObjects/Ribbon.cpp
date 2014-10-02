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
#include "../SceneElement/FloatKey.h"
#include "../Ribbon/RibbonCurveDestroy.h"
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
	m_finishComposeTime(0.0f),
	m_ribbonWeightCurve(NULL)
{
	assert(ribbonData != NULL);
	assert(ribbonData->Path != NULL);
	assert(ribbonData->Path->Keys.size() > 0);

	if (ribbonData->Destination != NULL)
		m_name = ribbonData->Destination->MeshName;

	int order = 1;
	int halfPathKeyIndex = ribbonData->Path->Keys.size() / 2;

	int keysCount = ribbonData->Path->Keys.size();
	float duration = ribbonData->Path->Keys[keysCount - 1]->Time - ribbonData->Path->Keys[0]->Time;

	m_startDecomposeTime = ribbonData->Path->Keys[0]->Time;
	m_finishComposeTime = ribbonData->Path->Keys[keysCount - 1]->Time + ribbonData->Path->Delay;

	Model* model = Content::Instance->Get<Model>(sceneName);
	assert(model != NULL);

	Shader* glowSpecullarShader = Content::Instance->Get<Shader>("SpecularBlur");
	assert(glowSpecullarShader != NULL);

	Shader* staticGlowSpecullarShader = Content::Instance->Get<Shader>("StaticSpecularBlur");
	assert(staticGlowSpecullarShader != NULL);

	IRibbonCurveBuilder* sourceCurveBuilder = NULL;
	IRibbonCurveBuilder* destinationCurveBuilder = NULL;

	if (ribbonData->Source != NULL && ribbonData->Source->Destroy)
	{
		sourceCurveBuilder = new RibbonCurveDestroy();
		m_finishComposeTime *= 10.0f;
	}
	else if (ribbonData->Source != NULL && ribbonData->Destination == NULL)
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
			ribbonData->Path->Spread,
			ribbonData->Path->TriangleScale,
			ribbonData->Path->Delay / (ribbonData->Source == NULL ? 1.0f : 2.0f));

		m_decomposeAndFly->SetTriangleModificator(BlinkAtStartAndEnd::GetInstance());

		m_decomposeAndFly->GetMesh()->SetGlowPower(ribbonData->Source->Material->SolidGlowPower); // wspolne dla solid i wire

		if (!ribbonData->Path->DontRender)
		{
			if (ribbonData->Source->Material->UseSolid)
			{
				GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
				material->SetGlowMultiplier(ribbonData->Source->Material->SolidGlowMultiplier);
				material->SetColor(sm::Vec4(ribbonData->Source->Material->Diffuse, ribbonData->Source->Material->Opacity));

				m_decomposeAndFlyRenderable = new Renderable(m_decomposeAndFly->GetMesh(), material, order);
				m_renderables.push_back(m_decomposeAndFlyRenderable);
			}

			if (ribbonData->Source->Material->UseWire)
			{
				GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
				material->SetGlowMultiplier(ribbonData->Source->Material->WireGlowMultiplier);
				material->SetColor(sm::Vec4(ribbonData->Source->Material->Diffuse, ribbonData->Source->Material->Opacity));
				material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

				m_decomposeAndFlyRenderableWire = new Renderable(m_decomposeAndFly->GetMesh(), material, order);
				m_renderables.push_back(m_decomposeAndFlyRenderableWire);
			}
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
			ribbonData->Path->Spread,
			ribbonData->Path->TriangleScale,
			ribbonData->Path->Delay);

		m_composeFromRibbon->GetMesh()->SetGlowPower(ribbonData->Destination->Material->SolidGlowPower); // wspolne dla solid i wire

		if (!ribbonData->Path->DontRender)
		{
			if (ribbonData->Destination->Material->UseSolid)
			{
				GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
				material->SetGlowMultiplier(ribbonData->Destination->Material->SolidGlowMultiplier);
				material->SetColor(sm::Vec4(ribbonData->Destination->Material->Diffuse, ribbonData->Destination->Material->Opacity));

				m_composeFromRibbonRenderable = new Renderable(m_composeFromRibbon->GetMesh(), material, order);
				m_renderables.push_back(m_composeFromRibbonRenderable);
			}

			if (ribbonData->Destination->Material->UseWire)
			{
				GlowTransparencySpecullar* material = new GlowTransparencySpecullar(glowSpecullarShader);
				material->SetGlowMultiplier(ribbonData->Destination->Material->WireGlowMultiplier);
				material->SetColor(sm::Vec4(ribbonData->Destination->Material->Diffuse, ribbonData->Destination->Material->Opacity));
				material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

				m_composeFromRibbonRenderableWire = new Renderable(m_composeFromRibbon->GetMesh(), material, order);
				m_renderables.push_back(m_composeFromRibbonRenderableWire);
			}
		}
	}

	if (ribbonData->StaticSource != NULL ||
		(ribbonData->Source != NULL) && ribbonData->Source->Stay)
	{
		SceneElement::Material* materialData = (ribbonData->StaticSource != NULL ? ribbonData->StaticSource->Material : ribbonData->Source->Material);
		std::string meshName = (ribbonData->StaticSource != NULL ? ribbonData->StaticSource->MeshName : ribbonData->Source->MeshName);

		assert(materialData != NULL);
		assert(materialData->UseSolid || materialData->UseWire);

		Mesh* mesh = model->FindMesh(meshName);
		assert(mesh != NULL);

		m_staticSource = new StaticTriangledMesh();
		m_staticSource->Initialize(mesh->meshParts[0]);

		m_staticSource->SetGlowPower(materialData->SolidGlowPower); // wspolne dla solid i wire

		if (materialData->UseSolid)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(materialData->SolidGlowMultiplier);
			material->SetColor(sm::Vec4(materialData->Diffuse, materialData->Opacity));

			m_staticSourceRenderable = new Renderable(m_staticSource, material, order);
			m_renderables.push_back(m_staticSourceRenderable);
		}

		if (materialData->UseWire)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(materialData->WireGlowMultiplier);
			material->SetColor(sm::Vec4(materialData->Diffuse, materialData->Opacity));
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			m_staticSourceRenderableWire = new Renderable(m_staticSource, material, order);
			m_renderables.push_back(m_staticSourceRenderableWire);
		}
	}

	if (ribbonData->StaticDestination != NULL ||
		(ribbonData->Destination != NULL) && ribbonData->Destination->Stay)
	{
		SceneElement::Material* materialData = (ribbonData->StaticDestination != NULL ? ribbonData->StaticDestination->Material : ribbonData->Destination->Material);
		std::string meshName = (ribbonData->StaticDestination != NULL ? ribbonData->StaticDestination->MeshName : ribbonData->Destination->MeshName);

		assert(materialData != NULL);
		assert(materialData->UseSolid || materialData->UseWire);

		Mesh* mesh = model->FindMesh(meshName);
		assert(mesh != NULL);

		m_staticDestination = new StaticTriangledMesh();
		m_staticDestination->Initialize(mesh->meshParts[0]);

		m_staticDestination->SetGlowPower(materialData->SolidGlowPower); // wspolne dla solid i wire

		if (materialData->UseSolid)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(materialData->SolidGlowMultiplier);
			material->SetColor(sm::Vec4(materialData->Diffuse, materialData->Opacity));

			m_staticDestinationRenderable = new Renderable(m_staticDestination, material, order);
			m_renderables.push_back(m_staticDestinationRenderable);
		}

		if (materialData->UseWire)
		{
			StaticGlowTransparencySpecullar* material = new StaticGlowTransparencySpecullar(staticGlowSpecullarShader);
			material->SetGlowMultiplier(materialData->WireGlowMultiplier);
			material->SetColor(sm::Vec4(materialData->Diffuse, materialData->Opacity));
			material->SetPolygonMode(BaseGlowTransparencySpecullar::PolygonMode_Lines);

			m_staticDestinationRenderableWire = new Renderable(m_staticDestination, material, order);
			m_renderables.push_back(m_staticDestinationRenderableWire);
		}
	}

	if (ribbonData->Path->RibbonWeights.size() > 0)
	{
		m_ribbonWeightCurve = new AnimationCurve<float>();

		for (uint32_t i = 0; i < ribbonData->Path->RibbonWeights.size(); i++)
		{
			m_ribbonWeightCurve->AddKeyframe(
				ribbonData->Path->RibbonWeights[i]->Time,
				ribbonData->Path->RibbonWeights[i]->Value);
		}

		m_ribbonWeightCurve->SmoothTangents();
	}
}

Ribbon::~Ribbon()
{
}

void Ribbon::Update(float time, float seconds)
{
	for (uint32_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->SetActive(false);

	if (time >= m_startDecomposeTime && time <= m_finishComposeTime + 0.0f) // TODO: + 1 to czas na blinkniecie
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

TrianglesRibbon* Ribbon::GetSource()
{
	return m_composeFromRibbon;
}
