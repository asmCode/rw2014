#pragma once

#include <vector>

class GameObject;
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

	void RenderGameObjects(const std::vector<GameObject*>& gameObjects);

private:
	int m_screenWidth;
	int m_screenHeight;

	Framebuffer* m_mainFrame;
	Framebuffer* m_downsampledFrame;

	Texture* m_mainRenderTexture;

	Quad* m_fullScreenQuad;
	Quad* m_halfScreenQuad;

	Shader* m_downsampleShader;
};
