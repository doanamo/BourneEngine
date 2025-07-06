#pragma once

enum class ExitCode : u32
{
    Success = 0,
    Failure,
    MemoryLeak,
    SetupFailed,
    TestsFailed,
    DiscoverTestsFailed,
    QueryTestsFailed,
};
