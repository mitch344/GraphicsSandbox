#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "../Utility/Utils.h"
#include "../ShaderConstants/ShaderConstants.h"

struct SpotLight
{
    void CreateConstantBuffer(ID3D11Device* device) {
        D3D11_BUFFER_DESC cbSpotDesc;
        ZeroMemory(&cbSpotDesc, sizeof(D3D11_BUFFER_DESC));
        cbSpotDesc.Usage = D3D11_USAGE_DEFAULT;
        cbSpotDesc.ByteWidth = sizeof(CONSTANT_BUFFER_SPOTLIGHT);
        cbSpotDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbSpotDesc.CPUAccessFlags = 0;
        cbSpotDesc.MiscFlags = 0;
        ThrowIfFailed(device->CreateBuffer(&cbSpotDesc, nullptr, mCBSpotLightBuffer.GetAddressOf()));
    }

    void UpdateAndSetToShader(ID3D11DeviceContext* deviceContext, UINT slot) {
        deviceContext->UpdateSubresource(mCBSpotLightBuffer.Get(), 0, nullptr, &mCBSpotLightData, 0, 0);
        deviceContext->PSSetConstantBuffers(slot, 1, mCBSpotLightBuffer.GetAddressOf());
    }

    CONSTANT_BUFFER_SPOTLIGHT mCBSpotLightData;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mCBSpotLightBuffer;
};