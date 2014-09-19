#pragma once

#include <Math/Matrix.h>
#include <Graphics/ITransformable.h>
#include <IO\BinaryReader.h>
#include <Math/Animation/AnimationCurve.h>
#include "ICamera.h"

class AnimCamera : public ITransformable, public ICamera
{
private:
	int id;
	std::string m_name;
	sm::Matrix view;
	sm::Matrix dummy;

	float fov;
	float trgDist;
	float m_nearClip;
	float m_farClip;
	AnimationCurve<float> *fovAnim;
	AnimationCurve<float> *distAnim;
	float lastKeyFrameTime;
	float startTime;

public:
	AnimCamera(int id);
	~AnimCamera(void);

	static AnimCamera* FromStream(BinaryReader &br);
	static AnimCamera* FromFile(const std::string &path);

	void SetLastKeyFrameTime(float time);
	void SetFirstKeyFrameTime(float time);
	float GetLastKeyFrameTime();
	float GetStartTime();
	bool IsCam();
	int GetId();
	std::string GetName() const;
	sm::Matrix& Transform();
	sm::Matrix& AnimTransform();
	sm::Matrix& BaseTransform();
	float GetNearClip();
	float GetFarClip();

	sm::Matrix GetViewMatrix();
	sm::Vec3 GetPosition();

	float GetFov(float time);
	float GetTargetDistance(float time);
};
