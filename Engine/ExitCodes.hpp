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

// #fixme: Tests currently report leaked memory due to static Test::Registry arrays.
// Expose delegate for subscribing into static exit so the array can be freed.
