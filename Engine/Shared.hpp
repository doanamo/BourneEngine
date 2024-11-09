#pragma once

// #todo: Hide some of these includes in specific sources that use them
#include <cstdlib> // std::malloc, std::free
#include <cstdarg> // std::va_list
#include <cstdio> // std::sprintf
#include <cstring> // std::memset
#include <atomic> // std::atomic
#include <mutex> // std::mutex
#include <bit> // std::countl_zero
#include <limits> // std::numeric_limits
#include <type_traits> // constexpr

#include "Build/Defines.hpp"
#include "Common/Defines.hpp"
#include "Memory/Defines.hpp"
#include "Platform/Defines.hpp"
#include "Graphics/Defines.hpp"

#include "Common/Compiler.hpp"
#include "Common/Types.hpp"
#include "Common/Debug/Debug.hpp"
#include "Common/Debug/Assert.hpp"
#include "Common/Standard.hpp"

#include "Common/Utility.hpp"
#include "Common/UniquePtr.hpp"
#include "Common/Optional.hpp"
#include "Common/Logger/Logger.hpp"
#include "Common/Containers/Array.hpp"
#include "Common/Containers/String.hpp"
#include "Common/Containers/StringView.hpp"
#include "Memory/Memory.hpp"
#include "Platform/Thread.hpp"
