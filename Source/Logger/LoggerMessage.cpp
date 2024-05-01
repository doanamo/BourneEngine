#include "Logger/Shared.hpp"
#include "Logger/LoggerMessage.hpp"

thread_local char LoggerMessage::m_buffer[];
