#pragma once

struct ExitCodes
{
    enum Type : u32
    {
        Success = 0,
        SetupFailed = 1,
        TestsFailed = 2,
        MemoryLeak = 3,
    };
};

// #fixme: Tests currently report leaked memory due to static Test::Registry arrays.
// Expose delegate for subscribing into static exit so the array can be freed.
