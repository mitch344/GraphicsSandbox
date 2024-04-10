#include "D3D11BasePipeline.h"

D3D11BasePipeline::D3D11BasePipeline(HINSTANCE hInstance) : MainWindow(hInstance)
{
}

D3D11BasePipeline::~D3D11BasePipeline()
{
    if (mImmediateContext)mImmediateContext->ClearState();
}

void D3D11BasePipeline::Initialize()
{
	MainWindow::Initialize();

    //Debug
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    //Swap Chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = WindowWidth;
    sd.BufferDesc.Height = WindowHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = WindowHandle;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = 0;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        0, 0, D3D11_SDK_VERSION, &sd, mSwapChain.GetAddressOf(), mDevice.GetAddressOf(), 0, mImmediateContext.GetAddressOf()));

    //BackBuffer
    Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
    ThrowIfFailed(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(BackBuffer.GetAddressOf())));
    ThrowIfFailed(mDevice->CreateRenderTargetView(BackBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf()));

    //Depth Stencil
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = WindowWidth;
    depthStencilDesc.Height = WindowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilDesc, 0, mDepthStencilBuffer.GetAddressOf()));
    ThrowIfFailed(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, mDepthStencilView.GetAddressOf()));

    //Viewport
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mViewport.Width = static_cast<float>(WindowWidth);
    mViewport.Height = static_cast<float>(WindowHeight);
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mImmediateContext->RSSetViewports(1, &mViewport);

    //Set Toplogy to Render Traingles
    mImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Build Shaders
    ThrowIfFailed(D3DCompileFromFile(VetexShaderFileName.c_str(), 0, 0, "main", "vs_5_0", 0, 0, mVertexBlob.GetAddressOf(), 0));
    ThrowIfFailed(D3DCompileFromFile(PixelShaderFileName.c_str(), 0, 0, "main", "ps_5_0", 0, 0, mPixelBlob.GetAddressOf(), 0));
    ThrowIfFailed(mDevice->CreateVertexShader(mVertexBlob->GetBufferPointer(), mVertexBlob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
    ThrowIfFailed(mDevice->CreatePixelShader(mPixelBlob->GetBufferPointer(), mPixelBlob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));
    mImmediateContext->VSSetShader(mVertexShader.Get(), 0, 0);
    mImmediateContext->PSSetShader(mPixelShader.Get(), 0, 0);

    //Constant Buffers
    //Vertex
    D3D11_BUFFER_DESC cbVertexDesc;
    ZeroMemory(&cbVertexDesc, sizeof(D3D11_BUFFER_DESC));
    cbVertexDesc.Usage = D3D11_USAGE_DEFAULT;
    cbVertexDesc.ByteWidth = sizeof(CONSTANT_BUFFER_PER_VERTEX_DATA);
    cbVertexDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbVertexDesc.CPUAccessFlags = 0;
    cbVertexDesc.MiscFlags = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&cbVertexDesc, nullptr, mCBPerVertexBuffer.GetAddressOf()));

    // Material
    D3D11_BUFFER_DESC cbMaterialDesc;
    ZeroMemory(&cbMaterialDesc, sizeof(D3D11_BUFFER_DESC));
    cbMaterialDesc.Usage = D3D11_USAGE_DEFAULT;
    cbMaterialDesc.ByteWidth = sizeof(CONSTANT_BUFFER_MATERIAL_DATA);
    cbMaterialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbMaterialDesc.CPUAccessFlags = 0;
    cbMaterialDesc.MiscFlags = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&cbMaterialDesc, nullptr, mCBMaterialBuffer.GetAddressOf()));

    //Texture Flags
    D3D11_BUFFER_DESC cbTextureFlagDesc;
    ZeroMemory(&cbTextureFlagDesc, sizeof(D3D11_BUFFER_DESC));
    cbTextureFlagDesc.Usage = D3D11_USAGE_DEFAULT;
    cbTextureFlagDesc.ByteWidth = sizeof(CONSTANT_BUFFER_TEXTURE_FLAGS_DATA);
    cbTextureFlagDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbTextureFlagDesc.CPUAccessFlags = 0;
    cbTextureFlagDesc.MiscFlags = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&cbTextureFlagDesc, nullptr, mCBTextureFlagBuffer.GetAddressOf()));

    //View
    D3D11_BUFFER_DESC cbViewDesc;
    ZeroMemory(&cbViewDesc, sizeof(D3D11_BUFFER_DESC));
    cbViewDesc.Usage = D3D11_USAGE_DEFAULT;
    cbViewDesc.ByteWidth = sizeof(CONSTANT_BUFFER_VIEW_BUFFER_DATA);
    cbViewDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbViewDesc.CPUAccessFlags = 0;
    cbViewDesc.MiscFlags = 0;
    ThrowIfFailed(mDevice->CreateBuffer(&cbViewDesc, nullptr, mCBViewBuffer.GetAddressOf()));

    mConstantBuffers = std::make_unique<PIXEL_SHADER_CONSTANT_BUFFERS>(
        &mCBMaterialData, mCBMaterialBuffer.Get(),
        &mCBTextureFlagData, mCBTextureFlagBuffer.Get()
    );

    //Input Layout
    D3D11_INPUT_ELEMENT_DESC InputLayoutDescription[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BITANGENT",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(InputLayoutDescription);
    ThrowIfFailed(mDevice->CreateInputLayout(InputLayoutDescription, numElements, mVertexBlob->GetBufferPointer(), mVertexBlob->GetBufferSize(), mInputLayout.GetAddressOf()));
    mImmediateContext->IASetInputLayout(mInputLayout.Get());

    //Timer
    mTimer = std::make_unique<Timer>();
}