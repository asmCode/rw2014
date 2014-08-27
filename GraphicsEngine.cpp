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
	m_quad(NULL),
	m_mainFrame(NULL),
	m_halfFrame(NULL),
	m_mainRenderTexture(NULL),
	m_glowTexture(NULL),
	m_downsampledGlowTexture(NULL),
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

	m_glowTexture = new Texture(
		m_screenWidth,
		m_screenHeight,
		32,
		NULL,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Linear,
		Texture::Filter_Linear,
		false);

	m_downsampledGlowTexture = new Texture(
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
	m_mainFrame->AttachColorTexture(m_mainRenderTexture->GetId(), 0);
	m_mainFrame->AttachColorTexture(m_glowTexture->GetId(), 1);
	m_mainFrame->Validate();

	m_halfFrame = new Framebuffer();
	m_halfFrame->Initialize(m_screenWidth / 2, m_screenHeight / 2, 32);

	m_quad = new Quad();
	m_quad->Initialize();

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

void GraphicsEngine::RenderGameObjects(const std::vector<Renderable*>& renderables)
{


	m_mainFrame->BindFramebuffer();
	glViewport(0, 0, m_screenWidth, m_screenHeight);

	GLenum enabledBuffers[2];
	enabledBuffers[0] = GL_COLOR_ATTACHMENT0;
	enabledBuffers[1] = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(2, enabledBuffers);

	glDepthMask(true);
	glColorMaski(0, true, true, true, true);
	glColorMaski(1, true, true, true, true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	for (uint32_t i = 0; i < renderables.size(); i++)
		DrawingRoutines::DrawWithMaterial(renderables[i]);

	/*
	m_halfFrame->BindFramebuffer();
	m_halfFrame->AttachColorTexture(m_glowTexture->GetId());
	m_halfFrame->Validate();

	glDepthMask(true);
	glColorMask(true, true, true, true);
	glViewport(0, 0, m_screenWidth / 2, m_screenHeight / 2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*/

	//DrawGlow(gameObjects);

	//Blur(m_glowTexture);

	//m_blitShader->UseProgram();
	//m_blitShader->SetTextureParameter("u_tex", 0, m_blurTextureStep2->GetId());

	Framebuffer::RestoreDefaultFramebuffer();

	/*
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(false);
	glColorMask(true, true, true, false);
	glDisable(GL_DEPTH_TEST);

	Quad::Setup();
	m_quad->Draw();
	Quad::Clean();
	*/

	m_halfFrame->BindFramebuffer();
	m_halfFrame->AttachColorTexture(m_downsampledGlowTexture->GetId());

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	Downsample(m_glowTexture);

	Blur(m_downsampledGlowTexture);

	Framebuffer::RestoreDefaultFramebuffer();
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(false);
	glColorMask(true, true, true, false);
	glDisable(GL_DEPTH_TEST);

	//m_blitShader->UseProgram();
	//m_blitShader->SetTextureParameter("u_tex", 0, m_blurTextureStep2->GetId());

	m_addShader->UseProgram();
	m_addShader->SetTextureParameter("u_tex1", 0, m_mainRenderTexture->GetId());
	m_addShader->SetTextureParameter("u_tex2", 1, m_blurTextureStep2->GetId());

	glViewport(0, 0, m_screenWidth, m_screenHeight);

	Quad::Setup();
	m_quad->Draw();
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
	m_quad->Draw();
	Quad::Clean();
}

void GraphicsEngine::Blur(Texture* srcTexture)
{
	m_halfFrame->AttachColorTexture(m_blurTextureStep1->GetId());

	glViewport(0, 0, m_screenWidth / 2, m_screenHeight / 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthMask(false);
	glColorMask(true, true, true, false);
	glDisable(GL_DEPTH_TEST);

	m_horiBlurShader->UseProgram();
	m_horiBlurShader->SetTextureParameter("u_tex", 0, srcTexture->GetId());
	m_horiBlurShader->SetParameter("u_texelSize", 1.0f / (float)srcTexture->GetWidth());

	Quad::Setup();
	m_quad->Draw();
	Quad::Clean();

	m_halfFrame->AttachColorTexture(m_blurTextureStep2->GetId());

	m_vertBlurShader->UseProgram();
	m_vertBlurShader->SetTextureParameter("u_tex", 0, m_blurTextureStep1->GetId());
	m_vertBlurShader->SetParameter("u_texelSize", 1.0f / (float)srcTexture->GetHeight());

	Quad::Setup();
	m_quad->Draw();
	Quad::Clean();
}

void GraphicsEngine::DrawGlow(const std::vector<Renderable*>& renderables)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glDisable(GL_BLEND);

	for (uint32_t i = 0; i < renderables.size(); i++)
		DrawingRoutines::DrawGlow(renderables[i]);
}
