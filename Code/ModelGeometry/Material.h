#pragma once
#include <unordered_map>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#pragma comment(lib, "d3d11.lib")

#include "../../Dependencies/DirectXTK-dec2023/Inc/WICTextureLoader.h"
#include "../ShaderConstants/ShaderConstants.h"
#include "../Utility/Utils.h"
#include "../Layouts/ResourceSlot.h"

enum class MaterialColors {
    Diffuse,
    Specular,
    Ambient,
    Emissive
};

enum class MaterialScalars {
    Opacity,
    Shininess,
    Reflectivity
};

enum class TextureMapTypes {
    DiffuseMap,
    SpecularMap,
    AmbientMap,
    EmissiveMap,
    NormalMap,
    BumpMap,
    DisplacementMap,
    StencilDecalMap,
    ReflectionMap,
    ShininessMap,
    AlphaMap
};

class Material {

public:
    Material(const std::string& name,
        const std::unordered_map<TextureMapTypes, std::string>& textureMapFiles,
        const std::unordered_map<MaterialColors, DirectX::XMFLOAT4>& materialComponents,
        const std::unordered_map<MaterialScalars, float>& scalars);

    const std::string& GetName() const;
    const std::unordered_map<TextureMapTypes, std::string>& GetTextureMapFiles() const;
    const std::unordered_map<MaterialColors, DirectX::XMFLOAT4>& GetMaterialComponents() const;
    const std::unordered_map<MaterialScalars, float>& GetScalars() const;

    void BuildTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string folderPathToTextures, std::unordered_map<std::string,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& TextureMapResources);

    void SetTextures(ID3D11DeviceContext* deviceContext, ID3D11Buffer* cbTextureFlagBuffer, CONSTANT_BUFFER_TEXTURE_FLAGS_DATA* cbTextureFlagData,
        std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& TextureMapResources);

    void SetMaterialComponents(CONSTANT_BUFFER_MATERIAL_DATA* cbMaterialData);

private:
    void SetTextureFlag(ID3D11Buffer* cbTextureFlagBuffer, CONSTANT_BUFFER_TEXTURE_FLAGS_DATA* cbTextureFlagData, TextureMapTypes type, bool value);
    void BindTextureToSlot(TextureMapTypes type, ID3D11ShaderResourceView* textureSRV, ID3D11DeviceContext* deviceContext);

    std::string Name;
    std::unordered_map<TextureMapTypes, std::string> TextureMapFiles;
    std::unordered_map<MaterialColors, DirectX::XMFLOAT4> MaterialComponents;
    std::unordered_map<MaterialScalars, float> Scalars;
};