#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

/* Light Shader Constants */

//Directional
struct alignas(128) CONSTANT_BUFFER_DIRECTIONAL_LIGHT
{
    DirectX::XMFLOAT3 position;
    float Padding1;
    DirectX::XMFLOAT3 diffuse;
    float Padding2;
    DirectX::XMFLOAT3 specular;
    float Padding3;
    DirectX::XMFLOAT3 ambient;
    float Padding4;
    float ambientIntensity;
};

//Point
struct alignas(128) CONSTANT_BUFFER_POINT_LIGHT
{
    DirectX::XMFLOAT3 ambient;
    DirectX::XMFLOAT3 diffuse;
    DirectX::XMFLOAT3 specular;
    DirectX::XMFLOAT3 position;
    float ambientIntensity;
    float attenuation;
};

//Spotlight
struct alignas(128) CONSTANT_BUFFER_SPOTLIGHT
{
    DirectX::XMFLOAT3 ambient;
    DirectX::XMFLOAT3 diffuse;
    DirectX::XMFLOAT3 specular;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 direction;
    float ambientIntensity;
    float attenuation;
    float coneAngle;
};

//Area
struct alignas(128) CONSTANT_BUFFER_AREA_LIGHT
{
    DirectX::XMFLOAT3 ambient;
    DirectX::XMFLOAT3 diffuse;
    DirectX::XMFLOAT3 specular;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 right;
    DirectX::XMFLOAT3 up;
    float ambientIntensity;
    float attenuation;
    float width;
    float height;
};

/* Vertex Shader Constants */
struct alignas(64) CONSTANT_BUFFER_PER_VERTEX_DATA
{
    DirectX::XMMATRIX WVP;
};

/* Pixel Shader Constants */
struct alignas(16) CONSTANT_BUFFER_VIEW_BUFFER_DATA
{
    DirectX::XMVECTOR ViewDirection;
};

struct alignas(128) CONSTANT_BUFFER_MATERIAL_DATA
{
    DirectX::XMFLOAT4 MaterialDiffuseColor;
    DirectX::XMFLOAT4 MaterialSpecularColor;
    DirectX::XMFLOAT4 MaterialAmbientColor;
    DirectX::XMFLOAT4 MaterialEmissive;
    float MateriaOpacity;
    float MateriaShininess;
    float MaterialReflectivity;
};

struct alignas(16) CONSTANT_BUFFER_TEXTURE_FLAGS_DATA
{
    bool DiffuseMap = false;
    bool SpecularMap = false;
    bool AmbientMap = false;
    bool EmissiveMap = false;
    bool NormalMap = false;
    bool BumpMap = false;
    bool DisplacementMap = false;
    bool StencilDecalMap = false;
    bool ReflectionMap = false;
    bool ShininessMap = false;
    bool AlphaMap = false;
};

struct PIXEL_SHADER_CONSTANT_BUFFERS
{
    CONSTANT_BUFFER_MATERIAL_DATA* cbMaterialData;
    ID3D11Buffer* cbMaterialBuffer;

    CONSTANT_BUFFER_TEXTURE_FLAGS_DATA* cbTextureFlagData;
    ID3D11Buffer* cbTextureFlagBuffer;

    PIXEL_SHADER_CONSTANT_BUFFERS()
        : cbMaterialData(nullptr),
        cbMaterialBuffer(nullptr),
        cbTextureFlagData(nullptr),
        cbTextureFlagBuffer(nullptr)
    {}

    PIXEL_SHADER_CONSTANT_BUFFERS(
        CONSTANT_BUFFER_MATERIAL_DATA* materialData,
        ID3D11Buffer* materialBuffer,
        CONSTANT_BUFFER_TEXTURE_FLAGS_DATA* textureFlagData,
        ID3D11Buffer* textureFlagBuffer)
        : cbMaterialData(materialData),
        cbMaterialBuffer(materialBuffer),
        cbTextureFlagData(textureFlagData),
        cbTextureFlagBuffer(textureFlagBuffer)
    {}
};