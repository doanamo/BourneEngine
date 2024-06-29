#pragma once

#include <cstdarg> // va_list
#include <cstdio> // vsprintf_s
#include <atomic> // std::atomic
#include <mutex> // std::mutex
#include <bit> // std::countl_zero

#include "Build/Version.hpp"
#include "Common/Defines.hpp"

#if defined(PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
    #undef Yield
#endif  

#include "Common/Types.hpp"
#include "Common/Utility.hpp"
#include "Common/UniquePtr.hpp"
#include "Common/Debug/Debug.hpp"
#include "Common/Debug/Assert.hpp"
#include "Common/Logger/Logger.hpp"
#include "Common/Containers/Array.hpp"
#include "Memory/Allocator.hpp"
#include "Platform/Thread.hpp"
