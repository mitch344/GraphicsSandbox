cbuffer ViewBuffer : register(b0)
{
    float4 ViewDirection;
}

cbuffer MaterialBuffer : register(b1)
{
    float4 MaterialDiffuseColor;
    float4 MaterialSpecularColor;
    float4 MaterialAmbientColor; 
    float4 MaterialEmissive;
    float MaterialOpacity;
    float MaterialShininess;
    float MaterialReflectivity;
};

cbuffer TextureFlags : register(b2)
{
    bool DiffuseMap;
    bool SpecularMap;
    bool AmbientMap;
    bool EmissiveMap;
    bool NormalMap;
    bool BumpMap;
    bool DisplacementMap;
    bool StencilDecalMap;
    bool ReflectionMap;
    bool ShininessMap;
    bool AlphaMap;
}

cbuffer DirectionalLight : register(b3)
{
    float3 DirLightPosition;
    float Padding1;
    float3 DirLightDiffuse;
    float Padding2;
    float3 DirLightSpecular;
    float Padding3;
    float3 DirLightAmbient;
    float Padding4;
    float DirLightAmbientIntensity;
};

SamplerState g_Sampler : register(s0);
Texture2D g_DiffuseTexture : register(t0);
Texture2D g_SpecularTexture : register(t1);
Texture2D g_AmbientTexture : register(t2);
Texture2D g_EmissiveTexture : register(t3);
Texture2D g_NormalTexture : register(t4);
Texture2D g_BumpTexture : register(t5);
Texture2D g_DisplacementTexture : register(t6);
Texture2D g_StencilDecaTexture : register(t7);
Texture2D g_ReflectionTexture : register(t8);
Texture2D g_ShininessTexture : register(t9);
Texture2D g_AlphaTexture : register(t10);

struct SHADER_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 color : COLOR;
};

float4 main(SHADER_INPUT input) : SV_TARGET
{
    //Light Attributes
    float3 lightPosition = DirLightPosition;
    float3 lightDiffuseColor = DirLightDiffuse;
    float ambientIntensity = DirLightAmbientIntensity;
    float3 lightAmbientColor = DirLightAmbient * ambientIntensity;
    float3 lightSpecularColor = DirLightSpecular;

    //Light Direction and Surface Normal
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(lightPosition);
    
    //Material Colors
    float3 ambientColor = MaterialAmbientColor.rgb;
    float3 diffuseColor = MaterialDiffuseColor.rgb;
    float3 specularColor = MaterialSpecularColor.rgb;
    
    
    //Load Texture Maps
    float3 diffuseMap = DiffuseMap ? g_DiffuseTexture.Sample(g_Sampler, input.texCoord) : float3(1, 1, 1);
    float3 specularMap = SpecularMap ? g_SpecularTexture.Sample(g_Sampler, input.texCoord).rgb : float3(1, 1, 1);
    float3 ambientMap = AmbientMap ? g_AmbientTexture.Sample(g_Sampler, input.texCoord).rgb : float3(1, 1, 1);
    
    diffuseColor = (diffuseColor != float3(0.0, 0.0, 0.0)) ? (DiffuseMap ? diffuseColor * diffuseMap : diffuseColor) : diffuseMap;
    specularColor = (specularColor != float3(0.0, 0.0, 0.0)) ? (SpecularMap ? specularColor * specularMap : specularColor) : specularMap;
    ambientColor = (ambientColor != float3(0.0, 0.0, 0.0)) ? (AmbientMap ? ambientColor * ambientMap : ambientColor) : ambientMap;
    
    //Blinn-Phong Reflection
    float3 viewDir = normalize(ViewDirection);
    float3 halfVec = normalize(lightDir + viewDir);
    float specularFactor = pow(dot(normal, halfVec), MaterialShininess);
    float3 specularContribution = specularFactor * specularColor * lightSpecularColor;

    //Lambertian Reflectance
    float diffuseFactor = max(0.0f, dot(normalize(normal), normalize(lightDir)));
    float3 diffuseContribution = diffuseColor * lightDiffuseColor * diffuseFactor;
    
    //Ambient Component
    float3 ambientLighting = lightAmbientColor * ambientColor;
    
    //Summation
    float3 finalColor = diffuseContribution + specularContribution + ambientLighting;

    return float4(finalColor, 1.0f);
}