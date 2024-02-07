#include "Scene.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    std::shared_ptr<Scene> app = std::shared_ptr<Scene>(new Scene(hInstance));
    app->Title = L"GraphicsSandbox";
    app->WindowWidth = 1920;
    app->WindowHeight = 1080;
    app->Initialize();
    app->Run();
}

/*
#include "VRScene.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    std::shared_ptr<VRScene> app = std::shared_ptr<VRScene>(new VRScene(hInstance));
    app->Title = L"VRGraphicsSandbox";
    app->WindowWidth = 1920;
    app->WindowHeight = 1080;
    app->Initialize();
    app->Run();
}
*/