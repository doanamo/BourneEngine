#include "Shared.hpp"
#include "TestObject.hpp"

std::atomic<u64> TestObject::s_copyCount = 0;
std::atomic<u64> TestObject::s_moveCount = 0;
std::atomic<u64> TestObject::s_constructCount = 0;
std::atomic<u64> TestObject::s_destructCount = 0;
std::atomic<u64> TestObject::s_instanceCount = 0;
