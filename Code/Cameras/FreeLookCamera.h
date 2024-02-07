#pragma once
#include "Camera.h"
#include <DirectXMath.h>

class FreeLookCamera : public Camera
{
public:
	FreeLookCamera(int height, int width);
	~FreeLookCamera();
	void UpdateViewMatrix() override;

	float Yaw = 0.0f;
	float Pitch = 0.0f;
	float MoveLeftRight = 0.0f;
	float MoveBackForward = 0.0f;
};