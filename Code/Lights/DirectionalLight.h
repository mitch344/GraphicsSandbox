#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "../Utility/Utils.h"
#include "../ShaderConstants/ShaderConstants.h"

struct DirectionalLight 
{
    void CreateConstantBuffer(ID3D11Device* device) {
        D3D11_BUFFER_DESC cbDirectionalDesc;
        ZeroMemory(&cbDirectionalDesc, sizeof(D3D11_BUFFER_DESC));
        cbDirectionalDesc.Usage = D3D11_USAGE_DEFAULT;
        cbDirectionalDesc.ByteWidth = sizeof(CONSTANT_BUFFER_DIRECTIONAL_LIGHT);
        cbDirectionalDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDirectionalDesc.CPUAccessFlags = 0;
        cbDirectionalDesc.MiscFlags = 0;
        ThrowIfFailed(device->CreateBuffer(&cbDirectionalDesc, nullptr, mCBDirectionalLightBuffer.GetAddressOf()));
    }

    void UpdateAndSetToShader(ID3D11DeviceContext* deviceContext, UINT slot) {
        deviceContext->UpdateSubresource(mCBDirectionalLightBuffer.Get(), 0, nullptr, &mCBDirectionalLightData, 0, 0);
        deviceContext->PSSetConstantBuffers(slot, 1, mCBDirectionalLightBuffer.GetAddressOf());
    }

    CONSTANT_BUFFER_DIRECTIONAL_LIGHT mCBDirectionalLightData;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mCBDirectionalLightBuffer;
};