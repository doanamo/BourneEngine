#pragma once

#include <cstddef> // std::max_align_t
#include <cstdlib> // std::malloc, std::free
#include <cstdarg> // std::va_list
#include <cstdio> // std::sprintf
#include <cstring> // std::memset
#include <cmath> // std::floor, std::ceil
#include <atomic> // std::atomic
#include <mutex> // std::mutex
#include <utility> // std::as_const
#include <tuple> // std::tuple
#include <variant> // std::variant
#include <optional> // std::optional
#include <type_traits> // constexpr

#include "Build/Defines.hpp"
#include "Common/Defines.hpp"
#include "Memory/Defines.hpp"
#include "Graphics/Defines.hpp"

#include "Common/Compiler.hpp"
#include "Common/Types.hpp"
#include "Common/Debug/Debug.hpp"
#include "Common/Debug/Assert.hpp"
#include "Platform/Memory.hpp"

#include "Common/Utility.hpp"
// #todo: Add ScopeGuard helper
#include "Common/Structures/Result.hpp"
#include "Common/Structures/Function.hpp"
#include "Common/Structures/UniquePtr.hpp"
#include "Common/Logger/Logger.hpp"
#include "Common/Containers/Array.hpp"
#include "Common/Containers/String.hpp"
#include "Common/Containers/StringView.hpp"
#include "Memory/Memory.hpp"
#include "Platform/Debug.hpp"
#include "Platform/Thread.hpp"
