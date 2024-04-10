#include "Material.h"

Material::Material(const std::string& name,
    const std::unordered_map<TextureMapTypes, std::string>& textureMapFiles,
    const std::unordered_map<MaterialColors, DirectX::XMFLOAT4>& materialComponents,
    const std::unordered_map<MaterialScalars, float>& scalars)
    : Name(name), TextureMapFiles(textureMapFiles),
    MaterialComponents(materialComponents), Scalars(scalars) {}

void Material::SetMaterialComponents(CONSTANT_BUFFER_MATERIAL_DATA* cbMaterialData)
{
    //Set the material components for the Material constant buffer
    for (const auto& [component, value] : MaterialComponents) {
        switch (component) {
        case MaterialColors::Diffuse:
            cbMaterialData->MaterialDiffuseColor = value;
            break;
        case MaterialColors::Specular:
            cbMaterialData->MaterialSpecularColor = value;
            break;
        case MaterialColors::Ambient:
            cbMaterialData->MaterialAmbientColor = value;
            break;
        case MaterialColors::Emissive:
            cbMaterialData->MaterialEmissive = value;
            break;
        default:
            break;
        }
    }
    for (const auto& [scalar, value] : GetScalars()) {
        switch (scalar) {
        case MaterialScalars::Opacity:
            cbMaterialData->MateriaOpacity = value;
            break;
        case MaterialScalars::Shininess:
            cbMaterialData->MateriaShininess = value;
            break;
        case MaterialScalars::Reflectivity:
            cbMaterialData->MaterialReflectivity = value;
        }
    }
}

void Material::SetTextures(ID3D11DeviceContext* deviceContext, ID3D11Buffer* cbTextureFlagBuffer, CONSTANT_BUFFER_TEXTURE_FLAGS_DATA* cbTextureFlagData,
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& TextureMapResources)
{
    //Reset Texture Flags to False/Default
    *cbTextureFlagData = CONSTANT_BUFFER_TEXTURE_FLAGS_DATA();

    for (auto& pair : TextureMapFiles)
    {
        TextureMapTypes type = pair.first;
        const std::string& value = pair.second;

        SetTextureFlag(cbTextureFlagBuffer, cbTextureFlagData, type, true);

        auto it = TextureMapResources.find(value);
        if (it != TextureMapResources.end())
        {
            ID3D11ShaderResourceView* textureSRV = it->second.Get();
            BindTextureToSlot(type, textureSRV, deviceContext);
        }
    }
}

void Material::SetTextureFlag(ID3D11Buffer* cbTextureFlagBuffer, CONSTANT_BUFFER_TEXTURE_FLAGS_DATA* cbTextureFlagData, TextureMapTypes type, bool value)
{
    switch (type)
    {
    case TextureMapTypes::DiffuseMap:
        cbTextureFlagData->DiffuseMap = value;
        break;
    case TextureMapTypes::SpecularMap:
        cbTextureFlagData->SpecularMap = value;
        break;
    case TextureMapTypes::AmbientMap:
        cbTextureFlagData->AmbientMap = value;
        break;
    case TextureMapTypes::EmissiveMap:
        cbTextureFlagData->EmissiveMap = value;
        break;
    case TextureMapTypes::NormalMap:
        cbTextureFlagData->NormalMap = value;
        break;
    case TextureMapTypes::BumpMap:
        cbTextureFlagData->BumpMap = value;
        break;
    case TextureMapTypes::DisplacementMap:
        cbTextureFlagData->DisplacementMap = value;
        break;
    case TextureMapTypes::StencilDecalMap:
        cbTextureFlagData->StencilDecalMap = value;
        break;
    case TextureMapTypes::ReflectionMap:
        cbTextureFlagData->ReflectionMap = value;
        break;
    case TextureMapTypes::ShininessMap:
        cbTextureFlagData->ShininessMap = value;
        break;
    case TextureMapTypes::AlphaMap:
        cbTextureFlagData->AlphaMap = value;
    }
}

void Material::BindTextureToSlot(TextureMapTypes type, ID3D11ShaderResourceView* textureSRV, ID3D11DeviceContext* deviceContext)
{
    int textureSlot = static_cast<int>(type) + DIFFUSE_TEXTURE_SLOT;
    deviceContext->PSSetShaderResources(textureSlot, 1, &textureSRV);
}

void Material::BuildTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string folderPathToTextures, std::unordered_map<std::string,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& TextureMapResources)
{
    // Iterate through the TextureMapTypes associated with this material
    for (auto& pair : TextureMapFiles)
    {
        // Extract the type and file name for the current texture map
        TextureMapTypes type = pair.first;
        const std::string& value = pair.second;

        // Check if this texture is already loaded to prevent duplicates
        auto it = std::find_if(TextureMapResources.begin(), TextureMapResources.end(),
            [&](const auto& texturePair) { return texturePair.first == value; });

        // Skip loading if the texture is already in the resource map
        if (it != TextureMapResources.end())
            continue;

        // Construct the full path to the texture files
        std::wstring fullPathString(folderPathToTextures.begin(), folderPathToTextures.end());
        std::wstring wideString(value.begin(), value.end());
        std::wstring textureFilePath = fullPathString + L"\\" + wideString;

        // Load the texture from file and add it to the resource map
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        
        ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, textureFilePath.c_str(), resource.GetAddressOf(), resourceView.GetAddressOf()));
        TextureMapResources[value] = std::move(resourceView);

        std::wstring debugMessage = L"Loaded Texture: " + std::wstring(value.begin(), value.end()) + L"\n";
        OutputDebugStringW(debugMessage.c_str());
    }
}

const std::string& Material::GetName() const
{
    return Name;
}

const std::unordered_map<TextureMapTypes, std::string>& Material::GetTextureMapFiles() const
{
    return TextureMapFiles;
}

const std::unordered_map<MaterialColors, DirectX::XMFLOAT4>& Material::GetMaterialComponents() const
{
    return MaterialComponents;
}

const std::unordered_map<MaterialScalars, float>& Material::GetScalars() const
{
    return Scalars;
}