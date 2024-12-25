#pragma once

#include <csignal>

#define DEBUG_BREAK() std::raise(SIGINT)
#define DEBUG_ABORT() std::abort()
