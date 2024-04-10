#pragma once
#include <Windows.h>

//Buffer (Vertex Shader)
constexpr UINT PER_VERTEX_SLOT = 0;

//Buffers (Pixel Shader)
constexpr UINT VIEW_BUFFER_SLOT = 0;
constexpr UINT MATERIAL_BUFFER_SLOT = 1;
constexpr UINT TEXTURE_FLAGS_SLOT = 2;

//Lights
constexpr UINT DIRECTIONAL_LIGHT_SLOT = 3;

//Samplers
constexpr UINT SAMPLER_SLOT = 4;

//Texture Maps
constexpr UINT DIFFUSE_TEXTURE_SLOT = 0;
constexpr UINT SPECULAR_TEXTURE_SLOT = 1;
constexpr UINT AMBIENT_TEXTURE_SLOT = 2;
constexpr UINT EMISSIVE_TEXTURE_SLOT = 3;
constexpr UINT NORMAL_TEXTURE_SLOT = 4;
constexpr UINT BUMP_TEXTURE_SLOT = 5;
constexpr UINT DISPLACEMENT_TEXTURE_SLOT = 6;
constexpr UINT STENCIL_DECA_TEXTURE_SLOT = 7;
constexpr UINT REFLECTION_TEXTURE_SLOT = 8;
constexpr UINT SHININESS_TEXTURE_SLOT = 9;
constexpr UINT ALPHA_TEXTURE_SLOT = 10;