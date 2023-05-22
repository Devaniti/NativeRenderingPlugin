#include "RenderAPI.h"
#include "PlatformBase.h"

// Direct3D 11 implementation of RenderAPI.

#if SUPPORT_D3D11

#include <assert.h>
#include <d3d11.h>
#include "Unity/IUnityGraphicsD3D11.h"


class RenderAPI_D3D11 : public RenderAPI
{
public:
	RenderAPI_D3D11();
	virtual ~RenderAPI_D3D11() override { }

	void ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces) final;
	
	void CopyBuffer(void* dst, void* src, int size) final;

private:
	ID3D11Device* m_Device;
    ID3D11DeviceContext* m_Context;
};

RenderAPI* CreateRenderAPI_D3D11()
{
	return new RenderAPI_D3D11();
}

RenderAPI_D3D11::RenderAPI_D3D11()
	: m_Device(nullptr)
	, m_Context(nullptr)
{
}

void RenderAPI_D3D11::ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces)
{
	switch (type)
	{
	case kUnityGfxDeviceEventInitialize:
	{
		IUnityGraphicsD3D11* d3d = interfaces->Get<IUnityGraphicsD3D11>();
		m_Device = d3d->GetDevice();
    	m_Device->GetImmediateContext(&m_Context);
		break;
	}
	case kUnityGfxDeviceEventShutdown:
	{
		SAFE_RELEASE(m_Context);
		SAFE_RELEASE(m_Device);
		break;
	}
	}
}

void RenderAPI_D3D11::CopyBuffer(void* dst, void* src, int size)
{
	ID3D11Buffer* dstBuffer = static_cast<ID3D11Buffer*>(dst);
	ID3D11Buffer* srcBuffer = static_cast<ID3D11Buffer*>(src);
	D3D11_BOX srcBox { 0, 0, 0, (UINT)size, 1, 1 };
	m_Context->CopySubresourceRegion(dstBuffer, 0, 0, 0, 0, srcBuffer, 0, &srcBox);
}


#endif // #if SUPPORT_D3D11
