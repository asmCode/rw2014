#pragma once

class BaseScene
{
public:
	BaseScene() {};
	virtual ~BaseScene() {}

	virtual bool Initialize() = 0;

	virtual bool Update(float time, float deltaTime) = 0;
	virtual bool Draw(float time, float deltaTime) = 0;

	virtual void Enter() {};
	virtual void Leave() {};
};

