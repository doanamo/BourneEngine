#pragma once

#include <cstdarg> // va_list
#include <cstdio> // vsprintf_s
#include <atomic> // std::atomic
#include <mutex> // std::mutex
#include <bit> // std::countl_zero

#include "Build/Version.hpp"
#include "Common/Defines.hpp"
#include "Graphics/Defines.hpp"

#ifdef PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
    #undef Yield
#endif

#ifdef GRAPHICS_DIRECT3D12
    #include <d3d12.h>
    #include <dxgi1_6.h>
    #include <D3Dcompiler.h>

    #include <wrl.h>
    using Microsoft::WRL::ComPtr;

    #ifdef CONFIG_DEBUG
        #include <d3d12sdklayers.h>
        #include <dxgidebug.h>
    #endif
#endif

#include "Common/Types.hpp"
#include "Common/Utility.hpp"
#include "Common/UniquePtr.hpp"
#include "Common/Debug/Debug.hpp"
#include "Common/Debug/Assert.hpp"
#include "Common/Logger/Logger.hpp"
#include "Common/Containers/Array.hpp"
#include "Memory/Memory.hpp"
#include "Platform/Thread.hpp"
