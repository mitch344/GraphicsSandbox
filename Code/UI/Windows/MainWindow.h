#pragma once
#include <windows.h>
#include <string>

class MainWindow
{
public:
	MainWindow(HINSTANCE);
	virtual ~MainWindow();
	bool Initialize();
	int Run();
	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

	int WindowWidth;
	int WindowHeight;
	std::wstring Title = L"Application";

protected:
	HWND WindowHandle;
	HINSTANCE WinInstance;
};