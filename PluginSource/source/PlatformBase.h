#pragma once

// Standard base includes, defines that indicate our current platform, etc.

#include <stddef.h>


// Which platform we are on?
// UNITY_WIN - Windows (regular win32)
// UNITY_OSX - Mac OS X
// UNITY_LINUX - Linux
// UNITY_IOS - iOS
// UNITY_TVOS - tvOS
// UNITY_ANDROID - Android
// UNITY_METRO - WSA or UWP
// UNITY_WEBGL - WebGL
// UNITY_EMBEDDED_LINUX - EmbeddedLinux OpenGLES
// UNITY_EMBEDDED_LINUX_GL - EmbeddedLinux OpenGLCore
#if _MSC_VER
	#define UNITY_WIN 1
#elif defined(__APPLE__)
    #if TARGET_OS_TV
        #define UNITY_TVOS 1
    #elif TARGET_OS_IOS
        #define UNITY_IOS 1
	#else
		#define UNITY_OSX 1
	#endif
#else
	#error "Unknown platform!"
#endif



// Which graphics device APIs we possibly support?
#if UNITY_WIN
	#define SUPPORT_D3D11 1
#elif UNITY_IOS || UNITY_TVOS || UNITY_OSX
	#define SUPPORT_METAL 1
#else
	#error Unknown platform
#endif

// COM-like Release macro
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(a) if (a) { a->Release(); a = NULL; }
#endif

