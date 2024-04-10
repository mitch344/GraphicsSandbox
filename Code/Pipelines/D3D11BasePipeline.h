#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <d3d11sdklayers.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

#include "../UI/Windows/MainWindow.h"
#include "../Utility/Utils.h"
#include "../Time/Timer.h"
#include "../ShaderConstants/ShaderConstants.h"

class D3D11BasePipeline : public MainWindow
{
public:
	D3D11BasePipeline(HINSTANCE);
	virtual ~D3D11BasePipeline();
	void Initialize();
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	/* Device Objects */
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mImmediateContext;

	/* SwapChain */
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

	/* Buffer */
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

	/* Depth Stencil */
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	/* View Port */
	D3D11_VIEWPORT mViewport;

	/* Shaders */
	std::wstring VetexShaderFileName = L".//Shaders/VertexShader.hlsl";
	std::wstring PixelShaderFileName = L".//Shaders/PixelShader.hlsl";
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> mVertexBlob;
	Microsoft::WRL::ComPtr<ID3D10Blob> mPixelBlob;

	/* Layouts and States */
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizeState;

	/* Shader Constant Objects */

	//Main Object
	std::unique_ptr<PIXEL_SHADER_CONSTANT_BUFFERS> mConstantBuffers;

	//Vertex Shader
	CONSTANT_BUFFER_PER_VERTEX_DATA mCBPerVertexData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCBPerVertexBuffer;

	//Material - Pixel Shader
	CONSTANT_BUFFER_MATERIAL_DATA mCBMaterialData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCBMaterialBuffer;

	//Texture Flags - Pixel Shader
	CONSTANT_BUFFER_TEXTURE_FLAGS_DATA mCBTextureFlagData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCBTextureFlagBuffer;

	//View - Pixel Shader
	CONSTANT_BUFFER_VIEW_BUFFER_DATA mCBViewData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCBViewBuffer;

	/* Timer */
	std::unique_ptr<Timer> mTimer;
};