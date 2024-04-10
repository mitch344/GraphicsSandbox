#pragma once
/*
* Pre prebuild binaries for Assimp -> https://kimkulling.itch.io/the-asset-importer-lib

You will also need to manually copy
C:\Program Files\Assimp\bin\x64\assimp-vc143-mt.dll
to the \x64\Debug folder of this project after installing the binaries

The project is already pointing to the default install location of these binaries for the inc and lib

Note: https://github.com/assimp/assimp/issues/2271 (I have fixed this by appling the NOMINMAX)

*/

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <regex> 
#pragma comment(lib, "assimp-vc143-mt.lib")

#include "../ModelGeometry/Mesh.h"
#include "../ModelGeometry/Material.h"
#include "../ModelGeometry/Model.h"

class AssimpLoader {
public:
    AssimpLoader();
    ~AssimpLoader();

    bool LoadModel(const std::string& fileName, std::unique_ptr<Model>& model);

private:
    void ProcessNode(aiNode* node, const aiScene* scene, std::unique_ptr<Model>& model);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    Material ProcessMaterial(aiMaterial* material);

private:
    Assimp::Importer importer;
    std::string GetFilenameFromPath(const aiString& path);
};