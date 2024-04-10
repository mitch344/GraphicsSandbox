#pragma once
#include "D3D11BasePipeline.h" 

class D3D11Pipeline : public D3D11BasePipeline
{
public:
	D3D11Pipeline(HINSTANCE);
	virtual ~D3D11Pipeline();
	void Initialize();
	virtual void Update() = 0;
	virtual void Draw() = 0;
	int Run();
};