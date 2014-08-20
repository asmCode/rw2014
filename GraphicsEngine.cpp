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
	m_mainFrame(NULL)
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

	m_mainFrame = new Framebuffer();
	m_mainFrame->Initialize(m_screenWidth, m_screenHeight, 32);
	m_mainFrame->BindFramebuffer();
	m_mainFrame->AttachColorTexture(m_mainRenderTexture->GetId());
	m_mainFrame->Validate();

	m_fullScreenQuad = new Quad();
	m_fullScreenQuad->Initialize(m_screenWidth, m_screenHeight);

	m_halfScreenQuad = new Quad();
	m_halfScreenQuad->Initialize(m_screenWidth / 2, m_screenHeight / 2);

	m_downsampleShader = Content::Instance->Get<Shader>("Downsample");
	assert(m_downsampleShader != NULL);
}

void GraphicsEngine::RenderGameObjects(const std::vector<GameObject*>& gameObjects)
{
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	m_mainFrame->BindFramebuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (uint32_t i = 0; i < gameObjects.size(); i++)
		DrawingRoutines::DrawWithMaterial(gameObjects[i]->GetRenderable());

	Framebuffer::RestoreDefaultFramebuffer();

	glViewport(0, 0, m_screenWidth / 2, m_screenHeight / 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_downsampleShader->UseProgram();
	m_downsampleShader->SetTextureParameter("u_tex", 0, m_mainRenderTexture->GetId());
	m_downsampleShader->SetParameter("u_texelSize", 1.0f / (float)m_screenWidth, 1.0f / (float)m_screenHeight);
	//m_downsampleShader->SetParameter("u_texelSize", 0.1f, 1.1f);
	//m_downsampleShader->SetParameter("u_texelSize", 1.0f / m_screenWidth, 1.0f / m_screenHeight);

	Quad::Setup();
	m_halfScreenQuad->Draw();
	Quad::Clean();

	/*sm::Matrix prevMvp = SpriteBatch::Instance->GetMvp();
	SpriteBatch::Instance->SetMvp(sm::Matrix::Ortho2DMatrix(0, m_screenWidth / 2.0f, 0, m_screenHeight / 2.0f));
	
	SpriteBatch::Instance->Begin();
	SpriteBatch::Instance->Draw(m_mainRenderTexture->GetId(), 0, 0, m_screenWidth / 2, m_screenHeight / 2);
	SpriteBatch::Instance->End();

	SpriteBatch::Instance->SetMvp(prevMvp);*/
}
