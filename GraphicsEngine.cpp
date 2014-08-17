#include "GraphicsEngine.h"
#include "GameObject.h"
#include "Renderable.h"
#include "DrawingRoutines.h"
#include <GL/glew.h>
#include <stdint.h>

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

void GraphicsEngine::RenderGameObjects(const std::vector<GameObject*>& gameObjects)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (uint32_t i = 0; i < gameObjects.size(); i++)
		DrawingRoutines::DrawWithMaterial(gameObjects[i]->GetRenderable());
}
