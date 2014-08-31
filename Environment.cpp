#include "Environment.h"

Environment* Environment::m_instance;

Environment::Environment()
{
	m_screenWidth = 0;
	m_screenHeight = 0;
}

Environment* Environment::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new Environment();

	return m_instance;
}

void Environment::SetScreenSize(unsigned width, unsigned height)
{
	this->m_screenWidth = width;
	this->m_screenHeight = height;
}

void Environment::SetBasePath(const std::string &basePath)
{
	this->m_basePath = basePath;
}

void Environment::SetWritePath(const std::string &writePath)
{
	this->m_writePath = writePath;
}

unsigned Environment::GetScreenWidth() const
{
	return m_screenWidth;
}

unsigned Environment::GetScreenHeight() const
{
	return m_screenHeight;
}

std::string Environment::GetBasePath() const
{
	return m_basePath;
}

std::string Environment::GetWritePath() const
{
	return m_writePath;
}
