#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#pragma comment(lib, "d3d11.lib")

#include "../../Dependencies/DirectXTK-dec2023/Inc/WICTextureLoader.h"
#include "../Layouts/VertexLayout.h"
#include "../Layouts/ResourceSlot.h"
#include "Material.h"

class Model;
class Material;

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::unique_ptr<Material> material);

    Mesh(const Mesh& other);

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<uint32_t>& GetIndices() const;
    const Material* GetMaterial() const;

    void BuildD3D11MeshBuffers(ID3D11Device* device);
    void BuildD3D11MeshTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string folderPathToTextures, std::unordered_map<std::string,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>&TextureMapResources);

    void Draw(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers,
        std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& TextureMapResources);

    void DrawNoTextures(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers);

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unique_ptr<Material> material;

    UINT Stride = sizeof(Vertex);
    UINT Offset = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
};