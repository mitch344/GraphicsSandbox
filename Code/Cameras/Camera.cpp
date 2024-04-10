#include "Camera.h"

DirectX::XMVECTOR Camera::GetPosition() const
{
	return Position;
}

DirectX::XMVECTOR Camera::GetViewDirection() const
{
	return ViewDirection;
}

DirectX::XMMATRIX Camera::GetViewProjection() const
{
	return XMMatrixMultiply(View, Projection);
}

DirectX::XMMATRIX Camera::GetProjection() const
{
	return Projection;
}

void Camera::SetPosition(float x, float y, float z)
{
	Position = DirectX::XMVectorSet(x, y, z, 0.0f);
}

void Camera::SetViewFrustrum(float fovY, float aspect, float nearZ, float farZ)
{
	FovY = fovY;
	Aspect = aspect;
	NearZ = nearZ;
	FarZ = farZ;
	Projection = DirectX::XMMatrixPerspectiveFovLH(FovY, Aspect, NearZ, FarZ);
}

void Camera::UpdateViewFrustrum()
{
	Projection = DirectX::XMMatrixPerspectiveFovLH(FovY, Aspect, NearZ, FarZ);
}