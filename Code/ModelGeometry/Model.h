#pragma once
#include <vector>
#include <memory>
#include <string>
#include <wrl.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "Material.h"
#include "Mesh.h"

class Mesh;
class Material;

class Model {
public:
    Model();
    ~Model();

    const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const;
    const std::vector<std::unique_ptr<Material>>& GetMaterials() const;
    void SetFilePath(const std::string& dir);
    const std::string& GetFilePath() const;

    void AddMesh(std::unique_ptr<Mesh> mesh);
    void AddMaterial(std::unique_ptr<Material> material);

    void BuildBuffers(ID3D11Device* device);
    void BuildTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string folderPathToTextures);

    void Draw(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers);
    void DrawNoTextures(ID3D11DeviceContext* deviceContext, PIXEL_SHADER_CONSTANT_BUFFERS* constantBuffers);

    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> TextureMapResources;

private:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;

    std::string FilePath;
    std::vector<std::unique_ptr<Mesh>> Meshes;
    std::vector<std::unique_ptr<Material>> Materials;
};