#pragma once

#ifndef CONFIG_RELEASE
    // #todo: This also cleans memory - should it be renamed? ENABLE_MEMORY_DEBUGGING?
    #define ENABLE_MEMORY_STATS // Track memory usage statistics
    #define ENABLE_MEMORY_FILL // Fill memory with debug patterns
#endif
