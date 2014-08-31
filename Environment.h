#pragma once

#include <string>

class Environment
{
private:
	static Environment *m_instance;

	unsigned m_screenWidth;
	unsigned m_screenHeight;
	std::string m_basePath;
	std::string m_writePath;
	
public:
	Environment();
	static Environment* GetInstance();
	
	void SetBasePath(const std::string &basePath);
	void SetWritePath(const std::string &writePath);
	void SetScreenSize(unsigned width, unsigned height);
	
	unsigned GetScreenWidth() const;
	unsigned GetScreenHeight() const;
	std::string GetBasePath() const;
	std::string GetWritePath() const;
};
