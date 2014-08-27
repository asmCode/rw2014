#pragma once

#include <vector>

class GameObject;
class Renderable;
class Framebuffer;
class Texture;
class Quad;
class Shader;

class GraphicsEngine
{
public:
	GraphicsEngine(int screenWidth, int screenHeight);
	virtual ~GraphicsEngine();

	void Initialize();

	void RenderGameObjects(const std::vector<Renderable*>& renderables);

private:
	int m_screenWidth;
	int m_screenHeight;

	Framebuffer* m_mainFrame;
	Framebuffer* m_halfFrame;

	Texture* m_mainRenderTexture;
	Texture* m_glowTexture;
	Texture* m_downsampledGlowTexture;
	Texture* m_blurTextureStep1;
	Texture* m_blurTextureStep2;

	Quad* m_quad;

	Shader* m_downsampleShader;
	Shader* m_blitShader;
	Shader* m_horiBlurShader;
	Shader* m_vertBlurShader;
	Shader* m_addShader;

	void Downsample(Texture* srcTexture);
	void Blur(Texture* srcTexture);

	void DrawGlow(const std::vector<Renderable*>& renderables);
};
