#pragma once

#include <cstdarg> // va_list
#include <cstdio> // vsprintf_s
#include <atomic> // std::atomic
#include <mutex> // std::mutex
#include <bit> // std::countl_zero

#include "Build/Version.hpp"
#include "Common/Defines.hpp"
#include "Common/Types.hpp"
#include "Common/Utility.hpp"
#include "Common/Debug/Debug.hpp"
#include "Common/Debug/Assert.hpp"
#include "Common/Platform/Platform.hpp"
#include "Common/Platform/Thread.hpp"
#include "Common/Containers/Array.hpp"
#include "Common/Logger/Logger.hpp"
#include "Memory/Memory.hpp"
