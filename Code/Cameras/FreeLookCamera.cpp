#include "FreeLookCamera.h"

FreeLookCamera::FreeLookCamera(int height, int width)
{
	float x = float(width);
	float y = float(height);

	Position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Target = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	SetViewFrustrum(0.4f * DirectX::XM_PI, x / y, 0.1f, 10000.0f);
}

FreeLookCamera::~FreeLookCamera()
{
}

void FreeLookCamera::UpdateViewMatrix()
{
    Rotation = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, 0);
    Target = DirectX::XMVector3Transform(DefaultForward, Rotation);
    Target = DirectX::XMVector3Normalize(Target);

    Right = DirectX::XMVector3TransformCoord(DefaultRight, Rotation);
    Forward = DirectX::XMVector3TransformCoord(DefaultForward, Rotation);
    Up = DirectX::XMVector3Cross(Forward, Right);

    Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(Right, MoveLeftRight));
    Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(Forward, MoveBackForward));

    MoveLeftRight = 0.0f;
    MoveBackForward = 0.0f;

    Target = DirectX::XMVectorAdd(Position, Target);
    View = DirectX::XMMatrixLookAtLH(Position, Target, Up);

    ViewDirection = Target;
}