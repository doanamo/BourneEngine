#pragma once

#define DEBUG_BREAK() std::raise(SIGINT)
#define DEBUG_ABORT() std::raise(SIGABRT)
