#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::unique_ptr<Material> material)
    : vertices(std::move(vertices)), indices(std::move(indices)), material(std::move(material)) {
}

Mesh::Mesh(const Mesh& other)
    : vertices(other.vertices), indices(other.indices),
    material(std::make_unique<Material>(*other.material)) {
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return vertices;
}

const std::vector<uint32_t>& Mesh::GetIndices() const
{
    return indices;
}

const Material* Mesh::GetMaterial() const
{
    return material.get();
}

void Mesh::BuildD3D11MeshBuffers(ID3D11Device* device)
{
    // Vertex Buffer
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = vertices.data();

    ID3D11Buffer* vertexBuffer;
    device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

    // Index Buffer
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(uint32_t) * static_cast<UINT>(indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexBufferData;
    ZeroMemory(&indexBufferData, sizeof(indexBufferData));
    indexBufferData.pSysMem = indices.data();

    ID3D11Buffer* indexBuffer;
    device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

    // Store the buffers
    VertexBuffer.Attach(vertexBuffer);
    IndexBuffer.Attach(indexBuffer);

}

void Mesh::BuildD3D11MeshTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string folderPathToTextures, std::unordered_map<std::string,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& TextureMapResources)
{
    material->BuildTextures(device, deviceContext, folderPathToTextures, TextureMapResources);
}

void Mesh::Draw(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers, 
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& TextureMapResources)
{
    //Set the Material Components for the pixel shader
    material->SetMaterialComponents(constantBuffers->cbMaterialData);

    //Set the Textures for the pixel shader
    material->SetTextures(deviceContext, constantBuffers->cbTextureFlagBuffer, constantBuffers->cbTextureFlagData, TextureMapResources);

    //Set the Texture flag/bools for the pixel shader
    deviceContext->UpdateSubresource(constantBuffers->cbTextureFlagBuffer, 0, nullptr, constantBuffers->cbTextureFlagData, 0, 0);
    deviceContext->PSSetConstantBuffers(TEXTURE_FLAGS_SLOT, 1, &constantBuffers->cbTextureFlagBuffer);

    deviceContext->UpdateSubresource(constantBuffers->cbMaterialBuffer, 0, nullptr, constantBuffers->cbMaterialData, 0, 0);
    deviceContext->PSSetConstantBuffers(MATERIAL_BUFFER_SLOT, 1, &constantBuffers->cbMaterialBuffer);

    deviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);
    deviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    UINT indexCount = static_cast<UINT>(indices.size());
    deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Mesh::DrawNoTextures(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers)
{
    ///Set the Material Components for the Pixel Shader
    material->SetMaterialComponents(constantBuffers->cbMaterialData);
    deviceContext->UpdateSubresource(constantBuffers->cbMaterialBuffer, 0, nullptr, constantBuffers->cbMaterialData, 0, 0);
    deviceContext->PSSetConstantBuffers(MATERIAL_BUFFER_SLOT, 1, &constantBuffers->cbMaterialBuffer);

    ///Set the Vertex and Index Buffer for the Mesh
    deviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);
    deviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    UINT indexCount = static_cast<UINT>(indices.size());

    //Draw the Index buffer
    deviceContext->DrawIndexed(indexCount, 0, 0);
}