#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "../Utility/Utils.h"
#include "../ShaderConstants/ShaderConstants.h"

struct AreaLight
{
    void CreateConstantBuffer(ID3D11Device* device) {
        D3D11_BUFFER_DESC cbAreaDesc;
        ZeroMemory(&cbAreaDesc, sizeof(D3D11_BUFFER_DESC));
        cbAreaDesc.Usage = D3D11_USAGE_DEFAULT;
        cbAreaDesc.ByteWidth = sizeof(CONSTANT_BUFFER_AREA_LIGHT);
        cbAreaDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbAreaDesc.CPUAccessFlags = 0;
        cbAreaDesc.MiscFlags = 0;
        ThrowIfFailed(device->CreateBuffer(&cbAreaDesc, nullptr, mCBAreaLightBuffer.GetAddressOf()));
    }

    void UpdateAndSetToShader(ID3D11DeviceContext* deviceContext, UINT slot) {
        deviceContext->UpdateSubresource(mCBAreaLightBuffer.Get(), 0, nullptr, &mCBAreaLightData, 0, 0);
        deviceContext->PSSetConstantBuffers(slot, 1, mCBAreaLightBuffer.GetAddressOf());
    }

    CONSTANT_BUFFER_AREA_LIGHT mCBAreaLightData;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mCBAreaLightBuffer;
};