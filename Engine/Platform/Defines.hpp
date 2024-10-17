#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef Yield

#include <wrl.h>
using Microsoft::WRL::ComPtr;
