#pragma once

#include "DemoOptions.h"
#include "CameraMode.h"
#include "GraphicsLibrary\CubeTexture.h"

#include <Core/GenericSingleton.h>

#include <Graphics/Content/Content.h>

#include <Graphics/Interpolators/InterpolatorFactory.h>
#include <Graphics/IGraphicsEngine.h>
#include "GraphicsLibrary/AnimCamera.h"
#include <Graphics/AnimationData.h>

#include "GraphicsLibrary\OpenglWindow.h"
#include "GraphicsLibrary\SteerCamera.h"
#include "GraphicsLibrary\IKeyboardCallback.h"

#include "Particles/ParticleEmmiter.h"
#include "GeometryBatch.h"

#include "ManCam.h"
#include "AnimCameraManager.h"
#include <Graphics/Interpolators/Interpolator.h>
#include <Graphics/Interpolators/InterpolatorFactory.h>
#include <Graphics/Content/IContentObserver.h>
#include <Utils/Randomizer.h>

#include <windows.h>
#include <vector>
#include <map>

class BaseScene;
class ScenesManager;
class SynchManager;

DWORD WINAPI EditableDemoThread(void *params);
class DemoController;
class EditableDemo;
class MachineScreen;
class Cigarette;
class EditableCamera;
class ManCam;
class ShadowMappingTest;
class LoadingScreen;
class ParticlesManager;
class CreditsDance;
class Framebuffer;
class DepthTexture;
class Blur;
class PostProcessing;
class Frustum;
class Screens;
class PropertySignal;
class Content;
class SpriteBatch;
class FontRenderer;
class DistortParticleHandler;
class GameObject;
class GraphicsEngine;

#define demo DemoController::GetInstance()

class DemoController :
	public IContentObserver,
	public IKeyboardCallback,
	public IGraphicsEngine,
	public GenericSingleton<DemoController>
{
	friend class GenericSingleton<DemoController>;
	friend class NormalMappingTest;
	friend class ShadowMappingTest;
	friend class LoadingScreen;
	friend class MachineScreen;

public:
	static const float GlowBufferWidthRatio;
	static const float GlowBufferHeightRatio;

	std::vector<GameObject*> m_gameObjects;

	float tmp_progress;

	Content *m_content;

	enum GeometryBatches
	{
		GeometryBatches_FactoryHall = 0,
		GeometryBatches_Passage,
		GeometryBatches_RobotLying,
		GeometryBatches_RobotMoving,
		GeometryBatches_MechArms,
		GeometryBatches_Count
	};

	GeometryBatch m_geoBatch[GeometryBatches_Count];

	std::string m_strBasePath;

	PropertySignal *m_fovSignal;

	float m_fovPower;

	ScenesManager* m_scenesManager;

	Model *m_mdl_teapot;

	Texture *m_bgTex;
	Shader *m_distortShader;

	ICamera *m_activeCamera;
	SynchManager* m_synchManager;

	/*ParticleEmmiter *m_particleEmmiter;
	DistortParticleHandler *m_distortParticleHandler;*/

	//Texture *m_particleTex;
	//Texture *m_distortParticleTex;

	SpriteBatch *m_spriteBatch;
	FontRenderer *m_fontRenderer;
	Shader *m_spriteShader;
	
	//Effect *electro;

	static Randomizer random;

	unsigned noiseTex;
	void InitElectroNoise();

	IInterpolator<float> *eyeBlinkAnim;
	IInterpolator<float> *eyeRangeAnim;

	float eyeBlinkVal;
	float eyeRangeVal;
	float electroNoiseVal;
	float camShakeVal;

	AnimCameraManager animCamsMng;
	AnimCameraManager m_lightCamsMng;
	ICamera *currentCamera;
	ICamera *m_currentLightCamera;

	float fade;

	float CalcFlash(float time, float ms);

	bool isPlaying;

	Texture *mask;

	bool errorOccured;
	MachineScreen *machineScreen;

	OpenglWindow *glWnd;
	ManCam manCam;

	float m_biasScale;
	float m_biasClamp;

	Animation *camerasAnimation;
	Animation *camerasFactoryAnimation;

	sm::Matrix m_lightViewMatrix;
	sm::Matrix m_lightProjMatrix;
	Frustum *frustum;
	sm::Matrix m_view;
	sm::Matrix m_proj;
	sm::Matrix m_viewProj;
	sm::Matrix glowProj;
	bool isStereo;

	ParticlesManager *m_particlesManager;

	PostProcessing *postProcessing;

	Framebuffer *blurFbo;
	Texture *targetTex0;

	float m_greetzDanceTime;

	bool firstupdate;

	float m_endScreenOpacity;

	DepthTexture *m_shadowMapTexture;
	Texture *m_dummyColorTexture;
	Framebuffer *m_shadowMappingFramebuffer;

	Blur *blur;
	Blur *dofBlur;

	Texture *m_mainFrameTexture;

	Framebuffer *m_distortionFramebuffer;
	Texture *m_distortionTexture;

	CubeTexture *m_envTexture;

	std::vector<MeshPart*> allMeshParts;

	std::vector<MeshPart*> m_opacityGlowObjects;
	std::vector<MeshPart*> m_solidGlowObjects;
	std::vector<MeshPart*> m_opacityNonGlowObjects;
	std::vector<MeshPart*> m_solidNonGlowObjects;
	std::vector<MeshPart*> m_shadowCasterObjects;

	void SortByOpacity(std::vector<MeshPart*> &meshParts);
	void FilterGlowObjects();

	ShadowMappingTest *shadowPass;
	LoadingScreen *loadingScreen;

	CameraMode cameraMode;

	GraphicsEngine* m_graphicsEngine;

	int nextId;

	float fov;

	DemoController();
	~DemoController();

	void SetOpenglParams();
	bool InitCg();

	bool AssignAssets();

	// IContentObserver interface
	void ErrorOccured(const std::string &errorMsg);
	void OnLoadingContentStarted(int stepsCount);
	void OnProgressStep();
	void OnLoadingFinished();

	template <typename T>
	void DeleteObject(T &obj)
	{
		if (obj != NULL)
		{
			delete obj;
			obj = NULL;
		}
	}

	void DrawText(const std::string &text, int x, int y, BYTE r = 255, BYTE g = 255, BYTE b = 255);
	float CalcFps(float ms);
	void DrawEngineStats();

	int width;
	int height;

	int flaps;
	float delay;
	float delayLimit;
	float fps;

	void DrawGlowTexture();
	void DrawShadowMap();

	void FrustumCulling(std::vector<MeshPart*> &meshParts);

public:
	void OnLeftMouseDown();
	void OnKeyDown(int keyCode);

	void AssignLightmapsToModels();

	void InitializeBlur();

	bool Initialize(bool isStereo, HWND parent, const char *title, int width, int height,
		int bpp, int freq, bool fullscreen, bool createOwnWindow);
	bool LoadContent(const char *basePath);
	bool BeforeStartSetups();
	void Release();

	bool Update(float time, float ms);
	bool Draw(float time, float ms);

	int GetNextId();

	bool HasGlowMaterial(MeshPart *meshPart);
	bool HasOpacityMaterial(MeshPart *meshPart);

	void LoadProperties(const std::string &filename);

	void SetAlwaysVisibility(
		const std::vector<Model*> &models);

	void FilterOpacityObjects(
		const std::vector<Model*> &models,
		std::vector<MeshPart*> &opacityMeshParts,
		std::vector<MeshPart*> &solidGlowMeshParts);

	void DrawPostProcess(int sharpTexId, int blurTexId, int glowTex, unsigned int shadowTex, float fade);

	Texture* LoadTexture(const std::string &path);
	Shader* LoadShader(const std::string &vertexShaderPath,
										const std::string &fragmentShaderPath);
	Model* LoadModel(const std::string &path);
	AnimationData* LoadAnimation(const std::string &path);
	Material* LoadMaterial(const std::string &path);

	AnimationCurve<float> m_endScreenAnim;
	AnimationCurve<float> m_startScreenAnim;
	AnimCamera* m_prevCamera;
	float nextFadeTime;
	Texture* m_fadeTex;
	Texture* m_endScreen;
	Texture* m_startScreen;
	Texture* m_mask;
};
