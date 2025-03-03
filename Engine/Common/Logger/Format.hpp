#pragma once

#if ENABLE_LOGGER

namespace Logger
{
    class Message;

    const char* Format(const Message& message);
};

#endif
