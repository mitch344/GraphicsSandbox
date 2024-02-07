#pragma once
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

class Mouse
{
public:
	Mouse();
	~Mouse();

	void Initialize(HINSTANCE hInstance, HWND hWnd);
	void GetState();
	bool ChangedPosition();
	LONG GetCurrentStateX() const;
	LONG GetCurrentStateY() const;
	LONG GetCurrentStateZ() const;
	DIMOUSESTATE GetCurrentState() const;
	void SetLastState(const DIMOUSESTATE);

private:
	LPDIRECTINPUT8 DirectInput;
	LPDIRECTINPUTDEVICE8 Device;
	DIMOUSESTATE LastState;
	DIMOUSESTATE CurrentState;
};