#include "Shared.hpp"
#include "TestObject.hpp"

std::atomic<u64> Test::Object::s_copyCount = 0;
std::atomic<u64> Test::Object::s_moveCount = 0;
std::atomic<u64> Test::Object::s_constructCount = 0;
std::atomic<u64> Test::Object::s_destructCount = 0;
std::atomic<u64> Test::Object::s_instanceCount = 0;
