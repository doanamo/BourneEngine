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
#include <type_traits> // constexpr
#include <bit> // std::countl_zero

#include "Build/Defines.hpp"
#include "Common/Defines.hpp"
#include "Memory/Defines.hpp"
#include "Platform/Defines.hpp"
#include "Graphics/Defines.hpp"

#include "Common/Compiler.hpp"
#include "Common/Types.hpp"
#include "Common/Debug/Debug.hpp"
#include "Common/Debug/Assert.hpp"
#include "Platform/Memory.hpp"

#include "Common/Utility/Utility.hpp"
#include "Common/Utility/NonCopyable.hpp"
#include "Common/Utility/ScopeGuard.hpp"
#include "Common/Utility/ScopeValue.hpp"
#include "Common/Utility/Result.hpp"
#include "Common/Utility/Optional.hpp"
#include "Common/Utility/Function.hpp"
#include "Common/Utility/UniquePtr.hpp"
#include "Common/Logger/Logger.hpp"
#include "Common/Containers/Array.hpp"
#include "Common/Containers/String.hpp"
#include "Common/Containers/StringView.hpp"
#include "Memory/Memory.hpp"
#include "Platform/Debug.hpp"
#include "Platform/Thread.hpp"
#include "Platform/Utility.hpp"

#include "Platform/Includes.hpp"
#include "Graphics/Includes.hpp"
