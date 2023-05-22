using UnityEngine;
using System;
using System.Runtime.InteropServices;
using UnityEngine.Rendering;
using System.Collections.Generic;

public class UseRenderingPlugin : MonoBehaviour
{
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("RenderingPlugin")]
#endif
	private static extern IntPtr GetRenderEventWithDataFunc();

	ComputeBuffer srcBuffer;
    ComputeBuffer dstBuffer;

    [StructLayout(LayoutKind.Sequential)]
    private class RegisterParticlesBuffersBridgeParams
    {
        public IntPtr dstBuffer;
        public IntPtr srcBuffer;
		public int size;
    }

	IntPtr renderEventData;

    Dictionary<Camera, CommandBuffer> cameraCommandBuffers = new Dictionary<Camera, CommandBuffer>();

	void RenderCallback(Camera cam)
	{
		if (!cameraCommandBuffers.ContainsKey(cam))
		{
			CommandBuffer commandBuffer = new CommandBuffer { name = "UseRenderingPlugin" };
            commandBuffer.Clear();
            commandBuffer.IssuePluginEventAndData(GetRenderEventWithDataFunc(), 0, renderEventData);
			cam.AddCommandBuffer(CameraEvent.BeforeForwardAlpha, commandBuffer);
            cameraCommandBuffers.Add(cam, commandBuffer);
		}
	}

    private void Start()
    {
        dstBuffer = new ComputeBuffer(128, sizeof(int));
        srcBuffer = new ComputeBuffer(128, sizeof(int));
        RegisterParticlesBuffersBridgeParams data = new RegisterParticlesBuffersBridgeParams();
        data.dstBuffer = dstBuffer.GetNativeBufferPtr();
        data.srcBuffer = srcBuffer.GetNativeBufferPtr();
        data.size = 128 * sizeof(int);
        renderEventData = Marshal.AllocHGlobal(Marshal.SizeOf(data));
        Marshal.StructureToPtr(data, renderEventData, true);
        Camera.onPreRender += RenderCallback;
    }
}
