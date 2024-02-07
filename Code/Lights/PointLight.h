#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "../Utility/Utils.h"
#include "../ShaderConstants/ShaderConstants.h"

struct PointLight
{
    void CreateConstantBuffer(ID3D11Device* device) {
        D3D11_BUFFER_DESC cbPointDesc;
        ZeroMemory(&cbPointDesc, sizeof(D3D11_BUFFER_DESC));
        cbPointDesc.Usage = D3D11_USAGE_DEFAULT;
        cbPointDesc.ByteWidth = sizeof(CONSTANT_BUFFER_POINT_LIGHT);
        cbPointDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbPointDesc.CPUAccessFlags = 0;
        cbPointDesc.MiscFlags = 0;
        ThrowIfFailed(device->CreateBuffer(&cbPointDesc, nullptr, mCBPointLightBuffer.GetAddressOf()));
    }

    void UpdateAndSetToShader(ID3D11DeviceContext* deviceContext, UINT slot) {
        deviceContext->UpdateSubresource(mCBPointLightBuffer.Get(), 0, nullptr, &mCBPointLightData, 0, 0);
        deviceContext->PSSetConstantBuffers(slot, 1, mCBPointLightBuffer.GetAddressOf());
    }

    CONSTANT_BUFFER_POINT_LIGHT mCBPointLightData;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mCBPointLightBuffer;
};