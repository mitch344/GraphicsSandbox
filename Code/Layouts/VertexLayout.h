#pragma once
#pragma warning(disable: 26495)

#include <DirectXMath.h>

struct Vertex
{
    Vertex() {}
    Vertex(float x, float y, float z,
        float u, float v,
        float nx, float ny, float nz,
        float tx, float ty, float tz,
        float bx, float by, float bz,
        float r, float g, float b, float a)
        : Position(x, y, z), TexCoord(u, v), Normal(nx, ny, nz),
        Tangent(tx, ty, tz), Bitangent(bx, by, bz),
        Color(r, g, b, a) {}

    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT2 TexCoord;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT3 Tangent;
    DirectX::XMFLOAT3 Bitangent;
    DirectX::XMFLOAT4 Color;
};