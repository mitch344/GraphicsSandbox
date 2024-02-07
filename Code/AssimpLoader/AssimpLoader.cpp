#include "AssimpLoader.h"

AssimpLoader::AssimpLoader() {}

AssimpLoader::~AssimpLoader() {}

bool AssimpLoader::LoadModel(const std::string& filePath, std::unique_ptr<Model>& model)
{
    model->SetFilePath(filePath);

    OutputDebugStringW(L"Loading Model...\n");

    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace |
        aiProcess_ConvertToLeftHanded);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return false;
    }

    ProcessNode(scene->mRootNode, scene, model);

    return true;
}

void AssimpLoader::ProcessNode(aiNode* node, const aiScene* scene, std::unique_ptr<Model>& model)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        Mesh mesh = ProcessMesh(aimesh, scene);
        model->AddMesh(std::make_unique<Mesh>(std::move(mesh)));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene, model);
    }
}

Mesh AssimpLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);

    std::vector<uint32_t> indices;
    indices.reserve(mesh->mNumFaces * 3);

    const bool hasPositions = mesh->HasPositions();
    const bool hasTextureCoords = mesh->HasTextureCoords(0);
    const bool hasNormals = mesh->HasNormals();
    const bool hasTangentsAndBitangents = mesh->HasTangentsAndBitangents();
    const bool hasColors = mesh->HasVertexColors(0);
    const bool hasFaces = mesh->HasFaces();

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        if (hasPositions) {
            const aiVector3D& pos = mesh->mVertices[i];
            vertex.Position = { pos.x, pos.y, pos.z };
        }

        if (hasTextureCoords) {
            const aiVector3D& texCoord = mesh->mTextureCoords[0][i];
            vertex.TexCoord = { texCoord.x, texCoord.y };
        }

        if (hasNormals) {
            const aiVector3D& normal = mesh->mNormals[i];
            vertex.Normal = { normal.x, normal.y, normal.z };
        }

        if (hasTangentsAndBitangents)
        {
            const aiVector3D& tangent = mesh->mTangents[i];
            const aiVector3D& bitangent = mesh->mBitangents[i];
            vertex.Tangent = { tangent.x, tangent.y, tangent.z };
            vertex.Bitangent = { bitangent.x, bitangent.y, bitangent.z };
        }

        if (hasColors)
        {
            const aiColor4D& color = mesh->mColors[0][i];
            vertex.Color = { color.r, color.g, color.b, color.a };
        }

        vertices.push_back(vertex);
    }

    if (hasFaces) {
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            const aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
    }

    Material material = ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex]);
    auto matPtr = std::make_unique<Material>(std::move(material));

    return Mesh(vertices, indices, std::move(matPtr));
}

Material AssimpLoader::ProcessMaterial(aiMaterial* material)
{
    //Materials

    aiString materialName;
    material->Get(AI_MATKEY_NAME, materialName);

    aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) != AI_SUCCESS)
    {
        diffuseColor = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    }

    aiColor4D specularColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) != AI_SUCCESS)
    {
        specularColor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
    }

    aiColor4D ambientColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) != AI_SUCCESS)
    {
        ambientColor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
    }

    aiColor4D emissiveColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) != AI_SUCCESS)
    {
        emissiveColor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
    }

    float opacity = 1.0f;
    if (material->Get(AI_MATKEY_OPACITY, opacity) != AI_SUCCESS)
    {
        opacity = 1.0f;
    }

    float shininess = 0.0f;
    if (material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS)
    {
        shininess = 0.0f;
    }

    float reflectivity = 0.0f;
    if (material->Get(AI_MATKEY_REFLECTIVITY, reflectivity) != AI_SUCCESS)
    {
        reflectivity = 0.0f;
    }


    std::unordered_map<TextureMapTypes, std::string> textureMapFiles;
    // Diffuse texture
    aiString diffuseTexturePath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::DiffuseMap] = GetFilenameFromPath(diffuseTexturePath);
    }

    // Specular texture
    aiString specularTexturePath;
    if (material->GetTexture(aiTextureType_SPECULAR, 0, &specularTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::SpecularMap] = GetFilenameFromPath(specularTexturePath);
    }

    // Ambient texture
    aiString ambientTexturePath;
    if (material->GetTexture(aiTextureType_AMBIENT, 0, &ambientTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::AmbientMap] = GetFilenameFromPath(ambientTexturePath);
    }

    // Emissive texture
    aiString emissiveTexturePath;
    if (material->GetTexture(aiTextureType_EMISSIVE, 0, &emissiveTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::EmissiveMap] = GetFilenameFromPath(emissiveTexturePath);
    }

    // Normal texture
    aiString normalTexturePath;
    if (material->GetTexture(aiTextureType_NORMALS, 0, &normalTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::NormalMap] = GetFilenameFromPath(normalTexturePath);
    }

    // Bump texture
    aiString bumpTexturePath;
    if (material->GetTexture(aiTextureType_HEIGHT, 0, &bumpTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::BumpMap] = GetFilenameFromPath(bumpTexturePath);
    }

    // Displacement texture
    aiString displacementTexturePath;
    if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &displacementTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::DisplacementMap] = GetFilenameFromPath(displacementTexturePath);
    }

    // Stencil Decal texture
    aiString stencilDecalTexturePath;
    if (material->GetTexture(aiTextureType_OPACITY, 0, &stencilDecalTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::StencilDecalMap] = GetFilenameFromPath(stencilDecalTexturePath);
    }

    // Reflection texture
    aiString reflectionTexturePath;
    if (material->GetTexture(aiTextureType_REFLECTION, 0, &reflectionTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::ReflectionMap] = GetFilenameFromPath(reflectionTexturePath);
    }

    // Shininess texture
    aiString shininessTexturePath;
    if (material->GetTexture(aiTextureType_SHININESS, 0, &shininessTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::ShininessMap] = GetFilenameFromPath(shininessTexturePath);
    }

    // Alpha texture
    aiString alphaTexturePath;
    if (material->GetTexture(aiTextureType_OPACITY, 0, &alphaTexturePath) == AI_SUCCESS)
    {
        textureMapFiles[TextureMapTypes::AlphaMap] = GetFilenameFromPath(alphaTexturePath);
    }

    return Material(std::string(materialName.C_Str()), textureMapFiles,
        std::unordered_map<MaterialColors, DirectX::XMFLOAT4>{
            {MaterialColors::Diffuse, DirectX::XMFLOAT4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a)},
            { MaterialColors::Specular, DirectX::XMFLOAT4(specularColor.r, specularColor.g, specularColor.b, specularColor.a) },
            { MaterialColors::Ambient, DirectX::XMFLOAT4(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a) },
            { MaterialColors::Emissive, DirectX::XMFLOAT4(emissiveColor.r, emissiveColor.g, emissiveColor.b, emissiveColor.a) }
    },
        std::unordered_map<MaterialScalars, float>{
            {MaterialScalars::Opacity, opacity},
            { MaterialScalars::Shininess, shininess },
            { MaterialScalars::Reflectivity, reflectivity }
    });
}

//Models are usually poorly fromated so will strip them.
std::string AssimpLoader::GetFilenameFromPath(const aiString& path)
{
    std::string filePath = path.C_Str();
    std::replace(filePath.begin(), filePath.end(), '\\', '/');
    filePath = std::regex_replace(filePath, std::regex("/+"), "/");
    size_t lastSlashPos = filePath.find_last_of('/');
    std::string filename = (lastSlashPos != std::string::npos) ? filePath.substr(lastSlashPos + 1) : filePath;
    return filename;
}