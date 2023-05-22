
#include "RenderAPI.h"
#include "PlatformBase.h"


// Metal implementation of RenderAPI.


#if SUPPORT_METAL

#include "Unity/IUnityGraphicsMetal.h"
#import <Metal/Metal.h>


class RenderAPI_Metal : public RenderAPI
{
public:
	RenderAPI_Metal();
	~RenderAPI_Metal() override { }

	void ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces) final;
	
	void CopyBuffer(void* dst, void* src, int size) final;
private:

private:
	IUnityGraphicsMetal* m_MetalGraphics;
    id<MTLDevice> m_MetalDevice;
};


RenderAPI* CreateRenderAPI_Metal()
{
	return new RenderAPI_Metal();
}

RenderAPI_Metal::RenderAPI_Metal()
{
}

void RenderAPI_Metal::ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces)
{
	if (type == kUnityGfxDeviceEventInitialize)
	{
		m_MetalGraphics = interfaces->Get<IUnityGraphicsMetal>();
    	m_MetalDevice = m_MetalGraphics->MetalDevice();
	}
	else if (type == kUnityGfxDeviceEventShutdown)
	{
	}
}

void RenderAPI_Metal::CopyBuffer(void* dst, void* src, int size)
{
	id<MTLBuffer> dstBuffer = (__bridge id<MTLBuffer>)dst;
	id<MTLBuffer> srcBuffer = (__bridge id<MTLBuffer>)src;
	m_MetalGraphics->EndCurrentCommandEncoder();
	id<MTLCommandBuffer> commandBuffer = m_MetalGraphics->CurrentCommandBuffer();
	id<MTLBlitCommandEncoder> blitCommandEncoder = [commandBuffer blitCommandEncoder];
    [blitCommandEncoder copyFromBuffer:srcBuffer
                          sourceOffset:0
                              toBuffer:dstBuffer
                     destinationOffset:0
                                  size:size];
	[blitCommandEncoder endEncoding];
}


#endif // #if SUPPORT_METAL
