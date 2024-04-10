cbuffer WVPBuffer : register(b0)
{
    float4x4 WVP;
}

struct SHADER_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 color : COLOR;
};

struct SHADER_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 color : COLOR;
};

SHADER_OUTPUT main(SHADER_INPUT input)
{
    SHADER_OUTPUT output;
    float4 transformedPosition = float4(input.position, 1.0f);
    
    transformedPosition = mul(transformedPosition, WVP);
    output.position = transformedPosition;
    output.texCoord = input.texCoord;
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.bitangent = input.bitangent;
    output.color = input.color;
    return output;
}