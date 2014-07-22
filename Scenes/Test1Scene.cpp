#include "Test1Scene.h"
#include <GL/glew.h>
//#include <gl/gl.h>

bool Test1Scene::Initialize()
{
	return true;
}

bool Test1Scene::Update(float time, float deltaTime)
{
	return true;
}

bool Test1Scene::Draw(float time, float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}
