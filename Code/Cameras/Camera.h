#pragma once
#include <DirectXMath.h>

class Camera
{
public:
    virtual ~Camera() {}
    void SetPosition(float x, float y, float z);
    void SetViewFrustrum(float fovY, float aspect, float nearZ, float farZ);
    void UpdateViewFrustrum();
    virtual void UpdateViewMatrix() = 0;

    DirectX::XMVECTOR GetPosition() const;
    DirectX::XMVECTOR GetViewDirection() const;
    DirectX::XMMATRIX GetProjection() const;
    DirectX::XMMATRIX GetViewProjection() const;


    DirectX::XMVECTOR Position;
    DirectX::XMVECTOR Up;
    DirectX::XMVECTOR Right;
    DirectX::XMVECTOR Target;
    DirectX::XMVECTOR Forward;
    DirectX::XMVECTOR ViewDirection;
    const DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    const DirectX::XMVECTOR DefaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    float NearZ;
    float FarZ;
    float Aspect;
    float FovY;
    float NearWindowHeight;
    float FarWindowHeight;
    DirectX::XMMATRIX Rotation;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Projection;
};