#include "GraphicsEngine.h"
#include "GameObject.h"
#include "Renderable.h"
#include "DrawingRoutines.h"
#include "Quad.h"
#include <Math/Vec2.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/SpriteBatch.h>
#include <Graphics/Texture.h>
#include <Graphics/Shader.h>
#include <Graphics/Content/Content.h>
#include <GL/glew.h>
#include <stdint.h>

GraphicsEngine::GraphicsEngine(int screenWidth, int screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_mainFrame(NULL),
	m_halfFrame(NULL),
	m_mainRenderTexture(NULL),
	m_halfRenderTexture(NULL),
	m_blurTextureStep1(NULL),
	m_blurTextureStep2(NULL),
	m_downsampleShader(NULL),
	m_blitShader(NULL),
	m_horiBlurShader(NULL),
	m_vertBlurShader(NULL),
	m_addShader(NULL)
{
}

GraphicsEngine::~GraphicsEngine()
{
}

void GraphicsEngine::Initialize()
{
	m_mainRenderTexture = new Texture(
		m_screenWidth,
		m_screenHeight,
		32,
		NULL,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Linear,
		Texture::Filter_Linear,
		false);

	m_halfRenderTexture = new Texture(
		m_screenWidth / 2,
		m_screenHeight / 2,
		32,
		NULL,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Linear,
		Texture::Filter_Linear,
		false);

	m_blurTextureStep1 = new Texture(
		m_screenWidth / 2,
		m_screenHeight / 2,
		32,
		NULL,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Linear,
		Texture::Filter_Linear,
		false);

	m_blurTextureStep2 = new Texture(
		m_screenWidth / 2,
		m_screenHeight / 2,
		32,
		NULL,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Linear,
		Texture::Filter_Linear,
		false);

	m_mainFrame = new Framebuffer();
	m_mainFrame->Initialize(m_screenWidth, m_screenHeight, 32);
	m_mainFrame->BindFramebuffer();
	m_mainFrame->AttachColorTexture(m_mainRenderTexture->GetId());
	m_mainFrame->Validate();

	m_halfFrame = new Framebuffer();
	m_halfFrame->Initialize(m_screenWidth / 2, m_screenHeight / 2, 32);

	m_fullScreenQuad = new Quad();
	m_fullScreenQuad->Initialize(m_screenWidth, m_screenHeight);

	m_halfScreenQuad = new Quad();
	m_halfScreenQuad->Initialize(m_screenWidth / 2, m_screenHeight / 2);

	m_downsampleShader = Content::Instance->Get<Shader>("Downsample");
	assert(m_downsampleShader != NULL);

	m_blitShader = Content::Instance->Get<Shader>("Blit");
	assert(m_blitShader != NULL);

	m_horiBlurShader = Content::Instance->Get<Shader>("HoriBlur");
	assert(m_horiBlurShader != NULL);

	m_vertBlurShader = Content::Instance->Get<Shader>("VertBlur");
	assert(m_vertBlurShader != NULL);

	m_addShader = Content::Instance->Get<Shader>("Add");
	assert(m_addShader != NULL);
}

void GraphicsEngine::RenderGameObjects(const std::vector<GameObject*>& gameObjects)
{
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	m_mainFrame->BindFramebuffer();

	glDepthMask(true);
	glColorMask(true, true, true, true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	for (uint32_t i = 0; i < gameObjects.size(); i++)
		DrawingRoutines::DrawWithMaterial(gameObjects[i]->GetRenderable());

	m_halfFrame->BindFramebuffer();
	m_halfFrame->AttachColorTexture(m_halfRenderTexture->GetId());
	m_halfFrame->Validate();

	Downsample(m_mainRenderTexture);

	m_halfFrame->AttachColorTexture(m_blurTextureStep1->GetId());
	Blur(m_halfRenderTexture);

	Framebuffer::RestoreDefaultFramebuffer();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(false);
	glColorMask(true, true, true, false);
	glDisable(GL_DEPTH_TEST);

	m_addShader->UseProgram();
	m_addShader->SetTextureParameter("u_tex1", 0, m_mainRenderTexture->GetId());
	m_addShader->SetTextureParameter("u_tex2", 1, m_blurTextureStep2->GetId());

	glViewport(0, 0, m_screenWidth, m_screenHeight);

	Quad::Setup();
	m_halfScreenQuad->Draw();
	Quad::Clean();
}

void GraphicsEngine::Downsample(Texture* srcTexture)
{
	glViewport(0, 0, m_screenWidth / 2, m_screenHeight / 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(false);
	glColorMask(true, true, true, false);
	glDisable(GL_DEPTH_TEST);

	m_downsampleShader->UseProgram();
	m_downsampleShader->SetTextureParameter("u_tex", 0, srcTexture->GetId());
	m_downsampleShader->SetParameter("u_texelSize", 1.0f / (float)m_screenWidth, 1.0f / (float)m_screenHeight);

	Quad::Setup();
	m_halfScreenQuad->Draw();
	Quad::Clean();
}

void GraphicsEngine::Blur(Texture* srcTexture)
{
	glViewport(0, 0, m_screenWidth / 2, m_screenHeight / 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(false);
	glColorMask(true, true, true, false);
	glDisable(GL_DEPTH_TEST);

	m_horiBlurShader->UseProgram();
	m_horiBlurShader->SetTextureParameter("u_tex", 0, srcTexture->GetId());
	m_horiBlurShader->SetParameter("u_texelSize", 1.0f / (float)srcTexture->GetWidth());

	Quad::Setup();
	m_halfScreenQuad->Draw();
	Quad::Clean();

	m_halfFrame->AttachColorTexture(m_blurTextureStep2->GetId());

	m_vertBlurShader->UseProgram();
	m_vertBlurShader->SetTextureParameter("u_tex", 0, m_blurTextureStep1->GetId());
	m_vertBlurShader->SetParameter("u_texelSize", 1.0f / (float)srcTexture->GetHeight());

	Quad::Setup();
	m_halfScreenQuad->Draw();
	Quad::Clean();
}
