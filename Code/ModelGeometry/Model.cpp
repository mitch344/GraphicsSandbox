#include "Model.h"

Model::Model() {}

Model::~Model() {}

const std::vector<std::unique_ptr<Mesh>>& Model::GetMeshes() const
{
    return Meshes;
}

const std::vector<std::unique_ptr<Material>>& Model::GetMaterials() const
{
    return Materials;
}

void Model::SetFilePath(const std::string& dir)
{
    FilePath = dir;
}

const std::string& Model::GetFilePath() const
{
    return FilePath;
}

void Model::AddMesh(std::unique_ptr<Mesh> mesh)
{
    Meshes.push_back(std::move(mesh));
}

void Model::AddMaterial(std::unique_ptr<Material> material)
{
    Materials.push_back(std::move(material));
}

void Model::BuildBuffers(ID3D11Device* device)
{
    for (const auto& mesh : Meshes)
    {
        mesh->BuildD3D11MeshBuffers(device);
    }
}

void Model::BuildTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string folderPathToTextures)
{
    //We need to set this some where else
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    device->CreateSamplerState(&sampDesc, SamplerState.GetAddressOf());
    deviceContext->PSSetSamplers(SAMPLER_SLOT, 1, SamplerState.GetAddressOf());

    for (const auto& mesh : Meshes)
    {
        mesh->BuildD3D11MeshTextures(device, deviceContext, folderPathToTextures, TextureMapResources);
    }
}

void Model::Draw(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers)
{
    for (const auto& mesh : Meshes)
    {
        mesh->Draw(deviceContext, constantBuffers, TextureMapResources);
    }
}

void Model::DrawNoTextures(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers)
{
    for (const auto& mesh : Meshes)
    {
        mesh->DrawNoTextures(deviceContext, constantBuffers);
    }
}