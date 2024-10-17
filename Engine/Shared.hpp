#pragma once

#include <cstdarg> // va_list
#include <cstdio> // vsprintf_s
#include <atomic> // std::atomic
#include <mutex> // std::mutex
#include <bit> // std::countl_zero
#include <limits> // std::numeric_limits
#include <immintrin.h>

#include "Build/Defines.hpp"
#include "Common/Defines.hpp"
#include "Common/Compiler.hpp"
#include "Memory/Defines.hpp"
#include "Graphics/Defines.hpp"

#ifdef PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
    #undef Yield
#endif

#ifdef GRAPHICS_DIRECT3D11
    #include <d3d11_4.h>
    #include <dxgi1_6.h>
    #include <d3dcompiler.h>

    #include <wrl.h>
    using Microsoft::WRL::ComPtr;

    #ifdef ENABLE_GRAPHICS_DEBUG
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
#include "Common/Containers/String.hpp"
#include "Common/Containers/StringView.hpp"
#include "Memory/Memory.hpp"
#include "Platform/Thread.hpp"
