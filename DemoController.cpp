#include "DemoController.h"
#include <Utils/Log.h>
#include "BasicLightingEffect.h"
#include "ParticlesManager.h"
#include "ShadowMappingTest.h"
#include <Graphics/Framebuffer.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include "LoadingScreen.h"
#include <Graphics/Shader.h>
#include "Blur.h"
#include "ManCam.h"
#include "XmlWriter.h"
#include <Graphics/DepthTexture.h>
#include <Graphics/MeshPart.h>
#include "Frustum.h"
#include <Graphics/BoundingSphere.h>
#include "common.h"
#include "MidiSynchEventsLoader.h"
#include "Scenes/Test1Scene.h"
#include "Scenes/GlowTestScene.h"
#include "Scenes/GuySceneTest.h"
#include "DemoUtils.h"
#include "Utils/Stopwatch.h"
#include "SynchManager.h"
#include "SynchEvent.h"

#include "ScenesManager.h"

#include "GraphicsEngine.h"
#include "AssemblingScene.h"
#include <Graphics/Property.h>
#include "PropertySignal.h"
#include "DrawingRoutines.h"
#include "Billboard.h"
#include "DistortParticleHandler.h"
#include "Particles/ParticleEmmiter.h"
#include "Particles/IParticleHandler.h"
#include "GameObject.h"
#include "Environment.h"
//#include "Dream.h"
#include "VectorGraphics.h"
#include "GraphicsLog.h"

#include <UserInput\Input.h>
#include <Graphics/TextureLoader.h>
#include <Graphics/ModelLoader.h>
#include <Graphics/MaterialLoader.h>
#include <Graphics/Shader.h>
#include <Graphics/AnimationLoader.h>
#include <Graphics/SpriteBatch.h>
#include <Graphics/FontRenderer.h>

#include <algorithm>

const float DemoController::GlowBufferWidthRatio = 0.5f;
const float DemoController::GlowBufferHeightRatio = 0.5f;

Texture *blackTex;
LinearInterpolator<float> fadeAnim;

DemoController* GenericSingleton<DemoController>::instance;
Randomizer DemoController::random;

  void APIENTRY OpenglDebugCallback(
	  GLenum source,
	  GLenum type,
	  GLuint id,
	  GLenum severity,
	  GLsizei length,
	  const GLchar* message,
	  void* userParam)
 {
	  int d;

	  switch (type)
	  {
	  case GL_DEBUG_TYPE_ERROR:
		  d = 0;
		  break;

	  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		  d = 0;
		  break;

	  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		  d = 0;
		  break;

	  case GL_DEBUG_TYPE_PORTABILITY:
		  d = 0;
		  break;

	  case GL_DEBUG_TYPE_PERFORMANCE:
		  d = 0;
		  break;

	  case GL_DEBUG_TYPE_OTHER:
		  d = 0;
		  break;

	  default:
		  d = 0;
		  break;
	  }
 }

float conv(int a, int b, int c)
{
	return a * 60.0f + b + ((float)c / 4800.0f);
}

DemoController::DemoController() :
	shadowPass(NULL),
	m_envTexture(NULL),
	m_scenesManager(NULL),
	m_fovSignal(NULL),
	m_fovPower(0.0f),
	m_synchManager(NULL)
{
	flaps = 0;
	firstupdate = true;
	fade = 0.0f;
	blurFbo = NULL;
	dofBlur = NULL;
	isPlaying = false;

	errorOccured = false;
	nextId = 0;

	glWnd = NULL;
	m_content = NULL;
	//scene = NULL;
	fov = 60.0f;

	isStereo = false;

	blur = NULL;

	m_biasScale = 0.0f;
	m_biasClamp = 0.0f;

	currentCamera = NULL;
	cameraMode = CameraMode_Free;
	m_shadowMapTexture = NULL;

	electroNoiseVal = 0.0f;
	camShakeVal = 0.0f;

	Log::StartLog(true, true, false);
}

DemoController::~DemoController()
{
	Release();
}

void DemoController::AssignLightmapsToModels()
{
	for (uint32_t i = 0; i < allMeshParts.size(); i++)
	{
		MeshPart *meshPart = allMeshParts[i];

		std::string lightmapName = allMeshParts[i]->mesh->name + "LightingMap";
		Texture *lightmap = m_content->Get<Texture>(lightmapName);

		if (lightmap != NULL &&
			!HasGlowMaterial(allMeshParts[i]) &&
			!(meshPart->material != NULL && meshPart->material->IsOpacity()))
			allMeshParts[i]->m_lightmap = lightmap;
	}
}

void DemoController::InitializeBlur()
{
	uint32_t glowWidth = static_cast<uint32_t>(width * GlowBufferWidthRatio);
	uint32_t glowHeight = static_cast<uint32_t>(height * GlowBufferHeightRatio);

	m_distortionTexture = new Texture(
		width,
		height,
		32,
		NULL,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Nearest,
		Texture::Filter_Nearest,
		false);

	m_mainFrameTexture = new Texture(
		width,
		height,
		32,
		NULL,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Nearest,
		Texture::Filter_Nearest,
		false);

	m_distortionFramebuffer = new Framebuffer();
	m_distortionFramebuffer->Initialize(width, height, 32);
	m_distortionFramebuffer->BindFramebuffer();
	m_distortionFramebuffer->AttachColorTexture(m_mainFrameTexture->GetId(), 0);
	m_distortionFramebuffer->AttachColorTexture(m_distortionTexture->GetId(), 1);
	m_distortionFramebuffer->Validate();
}

bool DemoController::Initialize(bool isStereo, HWND parent, const char *title, int width, int height,
	int bpp, int freq, bool fullscreen, bool createOwnWindow)
{
	delay = 0.0f;
	delayLimit = 0.5f;
	fps = 0.0f;

	tmp_progress = 0.0f;

	frustum = new Frustum();

	this->width = width;
	this->height = height;
	this->isStereo = isStereo;

	glWnd = new OpenglWindow();
	if (!glWnd->Initialize(parent, title,
		width, height, bpp, freq, fullscreen, createOwnWindow, dynamic_cast<IKeyboardCallback*>(this)))
	{
		Log::LogT("couldn't initialize opengl window");
		Release();
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		assert(false);
	}

	wglSwapIntervalEXT(false);

	std::string openGlVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	std::string openGlversion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	Log::LogT("OpenGl Vendor: %s", openGlVendor.c_str());
	Log::LogT("OpenGl Version: %s", openGlversion.c_str());

	SetOpenglParams();

#if 1
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenglDebugCallback, this);
#endif

	GraphicsLog::Initialize(NULL, NULL);

	loadingScreen = new LoadingScreen();

	targetTex0 = new Texture(width, height, 32, NULL, Texture::Wrap_ClampToEdge, Texture::Filter_Nearest, Texture::Filter_Nearest, false);

	blurFbo = new Framebuffer();
	blurFbo ->Initialize(width / 2, height / 2, 32);

	uint8_t fadeTexData[] =
	{
		0, 0, 0, 255,
		0, 0, 0, 255,
		0, 0, 0, 255,
		0, 0, 0, 255
	};

	m_fadeTex = new Texture(2, 2, 32, fadeTexData, Texture::Wrap_ClampToEdge, Texture::Filter_Nearest, Texture::Filter_Nearest, false);

	m_shadowMapTexture = new DepthTexture(width, height);
	m_dummyColorTexture = new Texture(width, height, 32, NULL, Texture::Wrap_ClampToEdge,
		Texture::Filter_Nearest, Texture::Filter_Nearest, false);

	m_shadowMappingFramebuffer = new Framebuffer();
	m_shadowMappingFramebuffer->Initialize(width, height, 32);
	m_shadowMappingFramebuffer->BindFramebuffer();
	m_shadowMappingFramebuffer->AttachColorTexture(m_dummyColorTexture->GetId());
	m_shadowMappingFramebuffer->AttachDepthTexture(m_shadowMapTexture->GetId());
	m_shadowMappingFramebuffer->Validate();
	Framebuffer::RestoreDefaultFramebuffer();

	Billboard::Initialize();

 //m_lightViewMatrix.a[0] = 0.8616f;
 //m_lightViewMatrix.a[1] = 0.2764f;
 //m_lightViewMatrix.a[2] = -0.4257f;
 //m_lightViewMatrix.a[3] = 0.0000f;
 //m_lightViewMatrix.a[4] = 0.0000f;
 //m_lightViewMatrix.a[5] = 0.8387f;
 //m_lightViewMatrix.a[6] = 0.5446f;
 //m_lightViewMatrix.a[7] = 0.0000f;
 //m_lightViewMatrix.a[8] = 0.5075f;
 //m_lightViewMatrix.a[9] = -0.4693f;
 //m_lightViewMatrix.a[10] = 0.7226f;
 //m_lightViewMatrix.a[11] = 0.0000f;
 //m_lightViewMatrix.a[12] = -0.0708f;
 //m_lightViewMatrix.a[13] = 1.4161f;
 //m_lightViewMatrix.a[14] = -7.5742f;
 //m_lightViewMatrix.a[15] = 1.0000f;

	//m_lightProjMatrix = sm::Matrix::Ortho2DMatrix(-10, 10, -10, 10);

	/*m_scenes.push_back(new Test1Scene());
	m_scenes.push_back(new GlowTestScene());
	m_scenes.push_back(new BoneAnimTestScene());
	m_scenes.push_back(new GuySceneTest());
	m_scenes.push_back(new CamsTestScene());
	m_scenes.push_back(new Scene01());
	m_scenes.push_back(new Scene03());

	m_activeScene = m_scenes[5];*/
	
	return true;
}

Animation *anim;

bool DemoController::LoadContent(const char *basePath)
{
	Stopwatch loadContentStopwatch(true);

	m_content = new Content(this);
	Content *dc = m_content;
	
	loadingScreen ->Initialize(basePath);
	loadingScreen->Draw();

	m_strBasePath = basePath;

	Environment::GetInstance()->SetBasePath(m_strBasePath);

	//dc->AddContentObserver(this);
	Stopwatch loadModelsStopwatch(true);
	dc->LoadModels(m_strBasePath + "models\\");
	Log::LogT("Models loaded in %.2f s", loadModelsStopwatch.GetTime());
	Stopwatch loadTexturesStopwatch(true);
	dc->LoadTextures(m_strBasePath + "textures\\");
	Log::LogT("Textures loaded in %.2f s", loadTexturesStopwatch.GetTime());
	Stopwatch loadShadersStopwatch(true);
	dc->LoadShaders(m_strBasePath + "effects\\");
	Log::LogT("Shaders loaded in %.2f s", loadShadersStopwatch.GetTime());
	Stopwatch loadAnimationsStopwatch(true);
	dc->LoadAnimations(m_strBasePath + "animations\\");
	Log::LogT("Animations loaded in %.2f s", loadAnimationsStopwatch.GetTime());
	Stopwatch loadMaterialsStopwatch(true);
	dc->LoadMaterials(m_strBasePath + "materials\\");
	Log::LogT("Materials loaded in %.2f s", loadMaterialsStopwatch.GetTime());
	Stopwatch loadSkinnedMeshesStopwatch(true);
	dc->LoadSkinnedMeshes(m_strBasePath + "SkinnedMeshes\\");
	Log::LogT("Skinned Meshes loaded in %.2f s", loadSkinnedMeshesStopwatch.GetTime());

	if (!AssignAssets())
		return false;

	Shader* vgShader = m_content->Get<Shader>("VectorGraphics");
	assert(vgShader != NULL);
	vgShader->BindVertexChannel(0, "a_position");
	vgShader->LinkProgram();

	VectorGraphics::Initialize(vgShader);

	m_spriteShader = m_content->Get<Shader>("Sprite");
	assert(m_spriteShader != NULL);
	m_spriteShader->BindVertexChannel(0, "a_position");
	m_spriteShader->BindVertexChannel(1, "a_coords");
	m_spriteShader->LinkProgram();

	m_spriteBatch = new SpriteBatch(m_spriteShader, sm::Matrix::Ortho2DMatrix(0, width, 0, height));

	m_endScreen = Content::Instance->Get<Texture>("end");
	assert(m_endScreen != NULL);

	m_startScreen = Content::Instance->Get<Texture>("loader");
	assert(m_startScreen != NULL);

	m_mask = Content::Instance->Get<Texture>("super_maska_kurwo");
	assert(m_mask != NULL);

	m_synchManager = new SynchManager();
	MidiSynchEventsLoader::LoadFromFile(m_strBasePath + "synch/Piano.snh", m_synchManager);
	m_synchManager->SortEventsByTime();

	m_endScreenAnim.AddKeyframe(5 * 60 + 56, 0);
	m_endScreenAnim.AddKeyframe(5 * 60 + 58, 1);
	m_endScreenAnim.SmoothTangents();

	m_startScreenAnim.AddKeyframe(0, 1);
	m_startScreenAnim.AddKeyframe(1.5f, 0);
	m_startScreenAnim.SmoothTangents();

	/*
	blackTex = dc->Get<Texture>("black");
	assert(blackTex != NULL);

	m_endScreen = dc->Get<Texture>("end");
	assert(m_endScreen);
	m_endScreenOpacity = 0.0f;

	InitializeBlur();

	m_spriteShader = m_content->Get<Shader>("Sprite");
	assert(m_spriteShader != NULL);
	m_spriteShader->BindVertexChannel(0, "a_position");
	m_spriteShader->BindVertexChannel(1, "a_coords");
	m_spriteShader->LinkProgram();

	m_spriteBatch = new SpriteBatch(m_spriteShader, sm::Matrix::Ortho2DMatrix(0, width, 0, height));
	m_fontRenderer = FontRenderer::LoadFromFile((m_strBasePath + std::string("fonts\\komika_title_32.xml")).c_str(), m_spriteBatch);

	Shader *distortShader = m_content->Get<Shader>("DistortParticle");
	assert(distortShader != NULL);
	distortShader->BindVertexChannel(0, "a_position");
	distortShader->LinkProgram();

	m_distortShader = m_content->Get<Shader>("Distortion");
	assert(m_distortShader != NULL);
	m_distortShader->BindVertexChannel(0, "a_position");
	m_distortShader->LinkProgram();

	Model *particlesModel = m_content->Get<Model>("smoke_sources");
	assert(particlesModel != NULL);

	m_particlesManager = new ParticlesManager();
	m_particlesManager->Initialize(particlesModel);

	anim = dc->Get<Animation>("animacja");
	Animation *headAnim = anim->GetAnimationByNodeName("Head");

	m_creditsDance = new CreditsDance();
	Robot *robot = new Robot();
	robot->SetCreditsDance(m_creditsDance);
	//m_gameObjects.push_back(new ShadowmapTest());
	m_gameObjects.push_back(robot);
	m_gameObjects.push_back(new Factory());
	m_gameObjects.push_back(m_creditsDance);
	m_gameObjects.push_back(new Fan());
	m_gameObjects.push_back(new SimpleAnim(dc->Get<Animation>("flying_chair"), dc->Get<Model>("flying_chair"), 2 * 60.0f + 14.0f + (265.0f / 4800.0f)));
	m_gameObjects.push_back(new SimpleAnim(dc->Get<Animation>("doors"), dc->Get<Model>("doors"), 0.0));
	m_gameObjects.push_back(new SimpleAnim(dc->Get<Animation>("kubek"), dc->Get<Model>("kubek"), 0.0));
	m_cigarette = new Cigarette(dc->Get<Animation>("epapieros"), dc->Get<Model>("epapieros"), 0.0);
	m_gameObjects.push_back(m_cigarette);

	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->Awake();

	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
	{
		std::vector<MeshPart*> &meshParts = m_gameObjects[i]->GetMeshParts();
		allMeshParts.insert(allMeshParts.end(), meshParts.begin(), meshParts.end());
	}

	AssignLightmapsToModels();
	SortByOpacity(allMeshParts);
	FilterGlowObjects();

	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->ClearLightmaps();
	}

	camerasAnimation = dc->Get<Animation>("cameras");
	assert(camerasAnimation != NULL);
	animCamsMng.Load(m_strBasePath + "cameras\\cameras.cam", camerasAnimation);

	camerasFactoryAnimation = dc->Get<Animation>("cameras_factory");
	assert(camerasFactoryAnimation != NULL);
	m_lightCamsMng.Load(m_strBasePath + "cameras\\cameras_factory.cam", camerasFactoryAnimation);

	*/

	Stopwatch loadScenesStopwatch(true);
	m_scenesManager = new ScenesManager();
	m_scenesManager->Initialize();
	Log::LogT("Scenes loaded in %.2f s", loadScenesStopwatch.GetTime());

	m_graphicsEngine = new GraphicsEngine(width, height);
	m_graphicsEngine->Initialize();

	BaseScene* scene = m_scenesManager->GetActiveScene();

	m_graphicsEngine->SetRenderables(scene->GetRenderables());

	Log::LogT("Loaded all assets in %.2f s", loadContentStopwatch.GetTime());

	return true;
}

void DemoController::Release()
{
	if (m_content != NULL)
	{
		delete m_content;
		m_content = NULL;
	}

	if (glWnd != NULL)
	{
		glWnd ->Release();
		delete glWnd;
		glWnd = NULL;
	}

	if (blur != NULL)
	{
		delete blur;
		blur = NULL;
	}

	if (dofBlur != NULL)
	{
		delete dofBlur;
		dofBlur = NULL;
	}

	Billboard::Release();

	DeleteObject(blurFbo);
	DeleteObject(m_envTexture);
}

static float lastTime;
bool DemoController::Update(float time, float seconds)
{
	if (Input::GetKey(KeyCode_LShift) && Input::GetKeyDown(KeyCode_Num1))
		DemoUtils::SaveCamera(&manCam, 0);

	if (!Input::GetKey(KeyCode_LShift) && Input::GetKeyDown(KeyCode_Num1))
		DemoUtils::LoadCamera(&manCam, 0);

	m_scenesManager->Update(time);

	m_activeCamera = NULL;

	BaseScene* activeScene = m_scenesManager->GetActiveScene();
	if (m_scenesManager->IsSceneChanged())
		m_graphicsEngine->SetRenderables(activeScene->GetRenderables());
		
	activeScene->Update(time, seconds);

#if MAN_CAM
	manCam.Process(seconds);
	m_activeCamera = &manCam;
#else
	//camerasAnimation->Update(m_greetzDanceTime, sm::Matrix::Identity, seconds);
	//m_activeCamera = animCamsMng.GetActiveCamera(time);
	m_activeCamera = activeScene->GetCamera();
#endif

	m_view = m_activeCamera->GetViewMatrix();
	m_proj = sm::Matrix::PerspectiveMatrix(
		m_activeCamera->GetFov(time),
		(float)width / (float)height,
		m_activeCamera->GetNearClip(),
		m_activeCamera->GetFarClip());

	m_viewProj = m_proj * m_view;
	return true;

	camerasFactoryAnimation->Update(time, sm::Matrix::Identity, seconds);
	m_currentLightCamera = m_lightCamsMng.GetActiveCamera(time);

	m_lightViewMatrix = m_currentLightCamera->GetViewMatrix();
	m_lightProjMatrix = sm::Matrix::PerspectiveMatrix(
		m_currentLightCamera->GetFov(time),
		(float)width / (float)height,
		m_currentLightCamera->GetNearClip(),
		m_currentLightCamera->GetFarClip());

// set camera wher light
#if 0 
	m_view = m_lightViewMatrix;
	m_proj = m_lightProjMatrix;
#endif

#if 0
	m_view.a[0] = 0.9890f;
	m_view.a[1] = -0.0625f;
	m_view.a[2] = -0.1340f;
	m_view.a[3] = 0.0000f;
	m_view.a[4] = 0.0000f;
	m_view.a[5] = 0.9063f;
	m_view.a[6] = -0.4226f;
	m_view.a[7] = 0.0000f;
	m_view.a[8] = 0.1478f;
	m_view.a[9] = 0.4180f;
	m_view.a[10] = 0.8964f;
	m_view.a[11] = 0.0000f;
	m_view.a[12] = -6.1468f;
	m_view.a[13] = -4.0603f;
	m_view.a[14] = -10.1110f;
	m_view.a[15] = 1.0000f;
#endif

	for (uint32_t i = 0; i < m_gameObjects.size(); i++)
		m_gameObjects[i]->Update(m_greetzDanceTime, seconds);

	/*m_proj = sm::Matrix::PerspectiveMatrix(
		deg(m_currentLightCamera->GetFov(0)),
		(float)width / (float)height,
		m_currentLightCamera->GetNearClip(),
		m_currentLightCamera->GetFarClip());

	m_view =
		sm::Matrix::RotateAxisMatrix(1.57f, 1, 0, 0) *
		m_currentLightCamera->GetViewMatrix();*/

	m_viewProj = m_proj * m_view;

	m_particlesManager->SetViewMatrix(m_view);
	m_particlesManager->SetProjMatrix(m_proj);
	m_particlesManager->Update(seconds);

	//anim->Update(time / 1000.0f, sm::Matrix::Identity, seconds);

	//m_robot->SetViewProjMatrix(m_viewProj);
	//m_robot->Update(time, seconds);

////	if (demoMode != DemoMode_Editor && !started)
////	{
//#ifndef DISABLE_MUSIC
//		music.SetPosition(time / 1000.0f);
//		music.Play();
//
//		//music.SetPosition(time / 1000.0f);
//#endif
////		started = true;
////	}
////
////	/*
////	if (!demoEnded)
////	{
////		time = music.GetPosition() * 1000.0f;
////
////		if ((time / 1000.0f) >= (180.0f + 43.0f))
////		{
////			music.Stop();
////			demoEnded = true;
////		}
////	}
////	*/
//
//	lastTime = time;
//
//	m_geoBatch[GeometryBatches_FactoryHall].SetVisibility(false);
//	m_geoBatch[GeometryBatches_MechArms].SetVisibility(false);
//	m_geoBatch[GeometryBatches_RobotLying].SetVisibility(false);
//	m_geoBatch[GeometryBatches_RobotMoving].SetVisibility(false);
//	m_geoBatch[GeometryBatches_Passage].SetVisibility(false);
//
//	return true;
//
//	m_doorsAnim->Update(time / 1000.0f, sm::Matrix::Identity, ms / 1000.0f);
//
//	m_fovPower -= (ms / 1000.0f) * 2.0f;
//	if (m_fovPower < 0.0f)
//		m_fovPower = 0.0f;
//
//	//m_fovPower = 1.0f;
//
//	if (m_fovSignal->CheckSignal(time / 1000.0f))
//	{
//		m_fovPower = 1.0f;
//	}
//
//	//if ((time / 1000.0f) >= 120.0f && m_activeScene != m_breakingWallScene)
//	//{
//	//	m_activeScene->SetVisibility(false);
//	//	m_breakingWallScene->SetVisibility(true);
//
//	//	m_activeScene = m_breakingWallScene;
//	//}
//
//	/*if (m_assemblingScene->IsActiveOnTime(time / 1000.0f) && (m_activeScene != m_assemblingScene))
//	{
//		m_assemblingScene->SetVisibility(true);
//
//		m_activeScene = m_assemblingScene;
//	}*/
//
//	camerasAnimation->Update(time / 1000, sm::Matrix::Identity, ms / 1000.0f);
//
//	if (errorOccured)
//		return false;
//
//	m_activeScene->Update(time / 1000.0f, ms / 1000.0f);
//
//#ifndef MAN_CAM
//	cameraMode = CameraMode_Preview;
//#endif
//	if (cameraMode == CameraMode_Preview) 
//	{
//		currentCamera = animCamsMng.GetActiveCamera(time / 1000.0f);
//		assert(currentCamera != NULL);
//	}
//
//	if (camShakeVal == 0.0f)
//		view = currentCamera->GetViewMatrix();
//	else
//		view =
//			sm::Matrix::RotateAxisMatrix(random.GetFloat(0.0f, 0.1f) * camShakeVal, sm::Vec3(random.GetFloat(-1, 1), random.GetFloat(-1, 1), random.GetFloat(-1, 1)).GetNormalized()) *
//			currentCamera->GetViewMatrix();
//
//	//view = view * sm::Matrix::ScaleMatrix(0.05f, 0.05f, 0.05f);
//
//	float camfov = deg(currentCamera ->GetFov(time / 1000.0f));
//
//	if (fov != camfov)
//	{
//		fov = camfov;
//
//		proj = sm::Matrix::PerspectiveMatrix(
//			fov, (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
//
//		/*glowProj = sm::Matrix::PerspectiveMatrix(
//			fov, (float)width / (float)height, NEAR_PLANE, FAR_PLANE);*/
//	}
//
//	if (dynamic_cast<ManCam*>(currentCamera) != NULL)
//		((ManCam*)currentCamera)->Process(ms);
//
//	//scene ->Update(time, ms);
//	//shadowPass->Update(time, ms);
//
#ifndef DISABLE_FRUSTUM_CULLING
	frustum->SetFrustum(
		m_activeCamera->GetViewMatrix(),
		m_activeCamera->GetNearClip(),
		m_activeCamera->GetFarClip(),
		m_activeCamera->GetFov(time),
		(float)width / (float)height);

	FrustumCulling(allMeshParts);
#endif

	return true;
}

static float fadeDir = 0.0f;

void DemoController::InitElectroNoise()
{
	int noiseHeight = height / 4;

	float *textureData = new float[noiseHeight];
	for (int i = 0; i < noiseHeight; i++)
		textureData[i] = ((float)(rand() % 200) - 100.0f) / 100.0f;

	glGenTextures(1, &noiseTex);
	glBindTexture(GL_TEXTURE_1D, noiseTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage1D(GL_TEXTURE_1D, 0, 1, noiseHeight, 0, GL_LUMINANCE, GL_FLOAT, textureData);
	glBindTexture(GL_TEXTURE_1D, 0);

	delete [] textureData;
}

float DemoController::CalcFlash(float time, float ms)
{
	AnimCamera *cam =
		dynamic_cast<AnimCamera*>(m_activeCamera);

	if (cam == NULL)
		return 0.0f;

	if (time > cam->GetLastKeyFrameTime() - 0.3f)
		fadeDir = 1.0f / 0.3f;

	fade = fade + (ms * fadeDir);

	if (fade >= 1.0f)
	{
		fade = 1.0f;
		fadeDir = -0.4f;
	}
	else if (fade <= 0.0f)
	{
		fade = 0.0f;
		fadeDir = 0.0f;
	}

	return fade;
}

bool DemoController::Draw(float time, float seconds)
{
	glViewport(0, 0, width, height);
	glDepthMask(true);
	glColorMask(true, true, true, true);

	DrawingRoutines::SetViewProjMatrix(m_viewProj);
	DrawingRoutines::SetLightPosition(sm::Vec3(0, 100, 100));
	DrawingRoutines::SetEyePosition(m_activeCamera->GetPosition());
	DrawingRoutines::SetLightPosition(m_activeCamera->GetPosition());

	//DrawingRoutines::DrawWithMaterial(m_content->Get<Model>("teapot")->m_meshParts);

	m_graphicsEngine->RenderGameObjects();

	m_graphicsEngine->RenderFullScreenTexture(m_mask, 0.5f);

	float fade = CalcFlash(time, seconds);

	if (fade > 0.0f)
		m_graphicsEngine->RenderFullScreenTexture(m_fadeTex, fade);

	if (time >= m_endScreenAnim.GetStartTime())
	{
		m_graphicsEngine->RenderFullScreenTexture(m_endScreen, m_endScreenAnim.Evaluate(time));
	}

	if (time <= m_startScreenAnim.GetEndTime())
	{
		m_graphicsEngine->RenderFullScreenTexture(m_startScreen, m_startScreenAnim.Evaluate(time));
	}

#if 0

	DrawShadowMap();

	m_distortionFramebuffer->BindFramebuffer();
	glViewport(0, 0, width, height);
	glDepthMask(true);
	glColorMask(true, true, true, true);
	
	GLenum enabledBuffers[2];
	enabledBuffers[0] = GL_COLOR_ATTACHMENT0;
	enabledBuffers[1] = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(2, enabledBuffers);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_lightViewMatrix = m_currentLightCamera->GetViewMatrix();
	
	DrawingRoutines::SetViewProjMatrix(m_viewProj);
	DrawingRoutines::SetLightPosition(sm::Vec3(0, 100, 100));
	DrawingRoutines::SetEyePosition(m_activeCamera->GetPosition());
	DrawingRoutines::SetLightPosition(m_activeCamera->GetPosition());
	DrawingRoutines::SetShadowLightPosition(m_lightViewMatrix.GetInversed() * sm::Vec3(0, 0, 0));

	//m_robot->Draw(time, seconds);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//DrawingRoutines::DrawWithMaterial(allMeshParts);
	if (m_creditsDance->IsActive())
		m_creditsDance->DrawOpacities();
	DrawingRoutines::DrawWithMaterialAndShadowMap(allMeshParts, m_shadowMapTexture->GetId());

	glDrawBuffers(2, enabledBuffers);

	/*m_spriteBatch->Begin();
	glDisable(GL_BLEND);
	m_spriteBatch->Draw(m_bgTex, 0, 0);
	m_spriteBatch->End();*/

	glEnable(GL_DEPTH_TEST);
	glDepthMask(false);

	m_particlesManager->Draw();
	m_cigarette->DrawSmokes(time, seconds);
	Framebuffer::RestoreDefaultFramebuffer();

	glDrawBuffer(GL_BACK);

	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	sm::Matrix mvp =
		sm::Matrix::Ortho2DMatrix(0, width, 0, height) *
		sm::Matrix::ScaleMatrix(static_cast<float>(width), static_cast<float>(height), 1.0f) *
		sm::Matrix::TranslateMatrix(0.5f, 0.5f, 0.0f);

	m_distortShader->UseProgram();
	m_distortShader->SetMatrixParameter("u_mvp", mvp);
	m_distortShader->SetTextureParameter("u_diffTex", 0, m_mainFrameTexture->GetId());
	m_distortShader->SetTextureParameter("u_distortTex", 1, m_distortionTexture->GetId());
	Billboard::Setup();
	Billboard::Draw();
	Billboard::Clean();
	
// glow stuff
#if 1

	DrawGlowTexture();

	glViewport(0, 0, width, height);
	m_spriteBatch->Begin();
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	m_spriteBatch->Draw(m_glowBlur->GetBlurredTexture(0), 0, 0, width, height);
	//m_spriteBatch->Draw(m_distortionTexture, 0, 0, width, height);
	//m_spriteBatch->Draw(m_mainFrameTexture, 0, 0, width, height);
	m_spriteBatch->End();
#endif

// testing texture
#if 0
	glViewport(0, 0, width, height);
	m_spriteBatch->Begin();
	glDisable(GL_BLEND);
	//m_spriteBatch->Draw(m_shadowMapTexture->GetId(), 0, 0, width / 2, height / 2);
	m_spriteBatch->Draw(m_dummyColorTexture->GetId(), 0, 0, width / 2, height / 2);
	m_spriteBatch->End();
#endif

	m_dream->Draw(time);

	float fadeVal = 0.0f;
	fadeAnim.GetValue(time, fadeVal);

	if (fadeVal != 0.0f)
	{
		glViewport(0, 0, width, height);
		m_spriteBatch->Begin();
		m_spriteBatch->Draw(blackTex, sm::Vec4(1, 1, 1, fadeVal), 0, 0, width, height);
		m_spriteBatch->End();
	}

	if (time > 256) // zaraz przed koncem ostatniej kamery
	{
		m_endScreenOpacity += 0.5f * seconds;
		if (m_endScreenOpacity > 1.0f)
			m_endScreenOpacity = 1.0f;
		glViewport(0, 0, width, height);
		m_spriteBatch->Begin();
		m_spriteBatch->Draw(m_endScreen, sm::Vec4(1, 1, 1, m_endScreenOpacity), 0, 0, width, height);
		m_spriteBatch->End();
	}
#endif

#ifdef SHOW_FPS
	glViewport(0, 0, width, height);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1, 0, 0, 1);
	float fps= CalcFps(seconds);
	char fpsText[1024];
	sprintf(fpsText, "fps: %.2f", fps);
	DrawText(fpsText, 4, height - 20, 255, 0, 0);
	DrawEngineStats();

	sm::Vec3 camPos = m_activeCamera->GetPosition();
	sprintf(fpsText, "camera position: (%.4f, %.4f, %.4f)", camPos.x, camPos.y, camPos.z);
	DrawText(fpsText, 4, 20, 255, 255, 255);

	sprintf(fpsText, "time: %.2f", time);
	DrawText(fpsText, 4, height - 160, 255, 0, 0);
	sprintf(fpsText, "fade: %.2f", fade);
	DrawText(fpsText, 4, height - 180, 255, 0, 0);
#endif

	VectorGraphics::SetViewProjMatrix(m_viewProj);
	GraphicsLog::DrawAndClear();

	glWnd ->SwapBuffers();

	return true;
}

void DemoController::SetOpenglParams()
{
	int width = glWnd ->GetWindowWidth();
	int height = glWnd ->GetWindowHeight();

	wglUseFontBitmaps(glWnd ->GetHDC(), 0, 255, 1000);
	glListBase(1000);

	glViewport(0, 0, width, height);

	//glowProj = sm::Matrix::PerspectiveMatrix(fov, (float)(width / 4) / (float)(height / 4), NEAR_PLANE, FAR_PLANE);

//	glShadeModel(GL_SMOOTH);

	//glClearColor(0.01f, 0.01f, 0.04f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

bool DemoController::AssignAssets()
{
	bool result = true;

	m_content->CombineResources();
	
	return result;
}

void DemoController::ErrorOccured( const std::string &errorMsg )
{
	Log::LogT("error: %s", errorMsg.c_str());
}

void DemoController::OnLoadingContentStarted(int stepsCount)
{
	loadingScreen ->SetStepsCount(stepsCount);
	loadingScreen ->Update(0, 0);
	//loadingScreen ->Draw(0, 0, view, proj, sm::Vec3(0, 0, 0));
}

void DemoController::OnProgressStep()
{
	loadingScreen ->ProgressStep();

	loadingScreen ->Update(0, 0);
	//loadingScreen ->Draw(0, 0, view, proj, sm::Vec3(0, 0, 0));
}

void DemoController::OnLoadingFinished()
{
}

bool DemoController::BeforeStartSetups()
{
	Content *dc = m_content;

	DrawingRoutines::Initialize(m_content);

	/*Shader *blurEffect = dc->Get<Shader>("blur");
	assert(blurEffect != NULL);
	blur = new Blur(3, blurEffect, width / 8, height / 8);
	dofBlur = new Blur(3, blurEffect, width / 4, height / 4);

	std::vector<Model*> models;*/

	//models.push_back(m_mdl_teapot);
	//models.push_back(m_mdl_factory);

	//models.push_back(m_doors);

	/*std::vector<Model*> &sceneModels1 = m_assemblingScene->GetModels();
	models.insert(models.end(), sceneModels1.begin(), sceneModels1.end());
	std::vector<Model*> &sceneModels2 = m_breakingWallScene->GetModels();
	models.insert(models.end(), sceneModels2.begin(), sceneModels2.end());*/

	//models.push_back(m_mdl_korytarz);
	
	//for (unsigned i = 0; i < models.size(); i++)
	//{
	//	std::vector<Mesh*> &meshes = models[i]->GetMeshes();
	//	for (unsigned j = 0; j < meshes.size(); j++)
	//	{
	//		std::vector<MeshPart*> &meshParts = meshes[j]->GetMeshParts();
	//		allMeshParts.insert(allMeshParts.begin(), meshParts.begin(), meshParts.end());
	//	}
	//}

	//FilterGlowObjects(models, glowMeshParts, nonGlowMeshParts);
	//FilterOpacityObjects(models, opacityMeshParts, solidMeshParts);
	//SetAlwaysVisibility(models);

	//for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	//{
	//	FilterGlowObjects(m_geoBatch[i].m_models, m_geoBatch[i].glowMeshParts, m_geoBatch[i].nonGlowMeshParts);
	//	FilterOpacityObjects(m_geoBatch[i].m_models, m_geoBatch[i].opacityMeshParts, m_geoBatch[i].solidMeshParts);
	//}

	////shadowPass = new ShadowMappingTest(width, height);
	////shadowPass->Initialize();

	return true;
}

int DemoController::GetNextId()
{
	nextId++;
	return nextId;
}

bool DemoController::HasGlowMaterial(MeshPart *meshPart)
{
	assert(meshPart != NULL);

	return
		meshPart->material != NULL &&
		meshPart->material->name.size() >= 4 &&
		meshPart->material->name.substr(0, 4) == "glow";
}

bool DemoController::HasOpacityMaterial(MeshPart *meshPart)
{
	assert(meshPart != NULL);

	return
		meshPart->material != NULL &&
		meshPart->material->IsOpacity();
}

void DemoController::DrawText(const std::string &text, int x, int y, BYTE r, BYTE g, BYTE b)
{
	//m_fontRenderer->DrawString("abcdefgh", 10, 10, Color::Blue);

	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(sm::Matrix::Ortho2DMatrix(0, width, 0, height).a);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glRasterPos2d(x, y);

	glColor3ub(r, g, b);
	glCallLists((int)text.size(), GL_UNSIGNED_BYTE, text.c_str());
	
	glPopAttrib();
}

float DemoController::CalcFps(float ms)
{
	flaps++;
	
	delay += ms;
	if (delay > delayLimit)
	{
		fps = (float)flaps / delay;
		delay = 0.0f;
		flaps = 0;
	}

	return fps;
}

bool MeshPartComp(MeshPart *&a, MeshPart *&b)
{
	float aOpacity = 1.0f;
	float bOpacity = 1.0f;

	if (a->material != NULL)
		aOpacity = a->material->Opacity();

	if (b->material != NULL)
		bOpacity = b->material->Opacity();

	return aOpacity > bOpacity;
}

void DemoController::SortByOpacity(std::vector<MeshPart*> &meshParts)
{
	std::sort(meshParts.begin(), meshParts.end(),  MeshPartComp);
}

void DemoController::FilterGlowObjects()
{
	for (unsigned i = 0; i < allMeshParts.size(); i++)
	{
		MeshPart *mp = allMeshParts[i];

		if (mp->mesh->m_isShadowCaster)
			m_shadowCasterObjects.push_back(mp);

		if (HasGlowMaterial(mp))
		{
			if (HasOpacityMaterial(mp))
			{
				m_opacityGlowObjects.push_back(mp);
			}
			else
			{
				m_solidGlowObjects.push_back(mp);
			}
		}
		else
		{
			if (HasOpacityMaterial(mp))
			{
				m_opacityNonGlowObjects.push_back(mp);
			}
			else
			{
				m_solidNonGlowObjects.push_back(mp);
			}
		}
	}
}


void DemoController::FilterOpacityObjects(const std::vector<Model*> &models,
										  std::vector<MeshPart*> &opacityMeshParts,
										  std::vector<MeshPart*> &solidGlowMeshParts)
{
	for (unsigned i = 0; i < models.size(); i++)
	{
		std::vector<Mesh*> &meshes = models[i] ->GetMeshes();

		for (unsigned j = 0; j < meshes.size(); j++)
		{
			std::vector<MeshPart*> &meshParts = meshes[j] ->GetMeshParts();

			for (unsigned k = 0; k < meshParts.size(); k++)
			{
				Material *mat = meshParts[k] ->GetMaterial();

				if (mat != NULL &&
					mat->IsOpacity())
					opacityMeshParts.push_back(meshParts[k]);
				else
					solidGlowMeshParts.push_back(meshParts[k]);
			}
		}
	}
}

void DemoController::DrawShadowMap()
{
	m_shadowMappingFramebuffer->BindFramebuffer();

	glDepthMask(true);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(false, false, false, false);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glViewport(0, 0, width, height);

	DrawingRoutines::SetShadowCastingLightView(m_lightViewMatrix);
	DrawingRoutines::SetShadowCastingLightProj(m_lightProjMatrix);
	
	DrawingRoutines::DrawShadowMap(m_shadowCasterObjects);
	
	Framebuffer::RestoreDefaultFramebuffer();
	
	glCullFace(GL_BACK);
}

void DemoController::FrustumCulling(std::vector<MeshPart*> &meshParts)
{
	for (unsigned i = 0; i < meshParts.size(); i++)
	{
		if (meshParts[i]->IsAlvaysVisible())
			continue;

		if (frustum->IsSphereInside(meshParts[i]->bsphere->center, meshParts[i]->bsphere->radius, *meshParts[i]->bbox))
			meshParts[i]->SetVisibility(true);
		else
			meshParts[i]->SetVisibility(false);
	}
}

void DemoController::DrawEngineStats()
{
	unsigned totalVerticesCount = 0;
	unsigned visibleVerticesCount = 0;
	unsigned totalMeshPartsCount = allMeshParts.size();
	for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	{
		if (m_geoBatch[i].IsVisible())
			totalMeshPartsCount += m_geoBatch[i].allMeshParts.size();
	}
	unsigned visibleMeshPartsCount = 0;

	for (unsigned i = 0; i < allMeshParts.size(); i++)
	{
		totalVerticesCount += allMeshParts[i]->GetVerticesCount();

		if (allMeshParts[i]->IsVisible())
		{
			visibleMeshPartsCount++;
			visibleVerticesCount += allMeshParts[i]->GetVerticesCount();
		}
	}

	for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	{
		for (unsigned j = 0; j < m_geoBatch[i].allMeshParts.size(); j++)
		{
			if (m_geoBatch[i].IsVisible())
			{
				totalVerticesCount += m_geoBatch[i].allMeshParts[j]->GetVerticesCount();

				if (m_geoBatch[i].allMeshParts[j]->IsVisible())
				{
					visibleMeshPartsCount++;
					visibleVerticesCount += m_geoBatch[i].allMeshParts[j]->GetVerticesCount();
				}
			}
		}
	}

	char text[1024];
	sprintf(text, "visible meshes: %u / %u (%.1f%%)", visibleMeshPartsCount, totalMeshPartsCount,
		100.0f * (float)visibleMeshPartsCount / (float)totalMeshPartsCount);
	DrawText(text, 4, height - 40, 255, 0, 0);
	sprintf(text, "visible vertices: %u / %u (%.1f%%)", visibleVerticesCount, totalVerticesCount,
		100.0f * (float)visibleVerticesCount / (float)totalVerticesCount);
	DrawText(text, 4, height - 60, 255, 0, 0);
}

void DemoController::SetAlwaysVisibility(const std::vector<Model*> &models)
{
	for (unsigned i = 0; i < models.size(); i++)
	{
		std::vector<Mesh*> &meshes = models[i] ->GetMeshes();

		for (unsigned j = 0; j < meshes.size(); j++)
		{
			if (meshes[j]->name.size() >= 4 &&
				meshes[j]->name.substr(0, 7) == "credits")
			{
				for (unsigned k = 0; k < meshes[j]->GetMeshParts().size(); k++)
				{
					meshes[j]->GetMeshParts()[k]->IsAlvaysVisible() = true;
					//meshes[j]->colorMask = sm::Vec3(2, 2, 2);
				}
			}
		}
	}
}

int propVal = 1;
static float timeShift = -1;

void DemoController::OnLeftMouseDown()
{
	if (timeShift == -1)
		timeShift = lastTime / 1000.0f;

	Log::LogT("changeGreetzProp->SetValue(%d, %.4f);", propVal, (lastTime / 1000.0f) - 0);

	if (propVal == 1)
		propVal = 0;
	else
		propVal = 1;
}

static int beatPropVal;

void DemoController::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case 'X':
		for (uint32_t i = 0; i < allMeshParts.size(); i++)
		{
			allMeshParts[i]->m_lightmap = NULL;
		}
		break;

	case 'T':
		m_biasScale += 0.0001f;
		tmp_progress += 0.01f;
		fov += 1.0f;
		break;

	case 'G':
		m_biasClamp += 0.0001f;
		tmp_progress -= 0.01f;
		fov -= 1.0f;
		break;

	case 'C':
		Log::LogT("Camera");
		for (uint32_t i = 0; i < 16; i++)
		{
			Log::LogT("m_view.a[%d] = %.4ff;", i, m_view.a[i]);
		}
		Log::LogT("");
		break;

	case 'L':
		Log::LogT("Light");
		for (uint32_t i = 0; i < 16; i++)
		{ 
			Log::LogT("lightTransform.a[%d] = %.4ff;", i, m_view.a[i]);
		}
		Log::LogT("");
		break;

	case 'B':
		if (beatPropVal == 0)
			beatPropVal = 1;
		else
			beatPropVal = 0;

		Log::LogT("m_fovAnim->SetValue(%d, %.4f);", beatPropVal, (lastTime / 1000.0f));
		break;
	}
}

Texture* DemoController::LoadTexture(const std::string &path)
{
	return TextureLoader::Load(path);
}

Shader* DemoController::LoadShader(const std::string &vertexShaderPath,
								      const std::string &fragmentShaderPath)
{
	Shader *shader = Shader::LoadFromFile(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

	return shader;
}

Model* DemoController::LoadModel(const std::string &path)
{
	return ModelLoader::LoadFromFile(path);
}

AnimationData* DemoController::LoadAnimation(const std::string &path)
{
	return AnimationLoader::LoadFromFile(path);
}

Material* DemoController::LoadMaterial(const std::string &path)
{
	return MaterialLoader::LoadFromFile(path);
}

