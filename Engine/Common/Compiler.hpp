#pragma once

#include <immintrin.h>

#define STRINGIFY(x) #x
#define EXPAND(x) x
#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define UNIQUE_NAME(base) CONCAT(base, __LINE__)
