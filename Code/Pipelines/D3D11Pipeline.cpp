#include "D3D11Pipeline.h"

D3D11Pipeline::D3D11Pipeline(HINSTANCE hInstance) : D3D11BasePipeline(hInstance)
{
}

D3D11Pipeline::~D3D11Pipeline()
{
}

void D3D11Pipeline::Initialize()
{
    D3D11BasePipeline::Initialize();
}

int D3D11Pipeline::Run()
{
    MSG msg = { 0 };

    mTimer->Reset();
    mTimer->Start();

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Update();
        Draw();
    }
    mTimer->Stop();
    return (int)msg.wParam;
}