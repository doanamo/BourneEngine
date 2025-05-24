#include "Shared.hpp"
#include "TestObject.hpp"

std::atomic<u64> Test::Object::s_instanceCurrentCount = 0;
std::atomic<u64> Test::Object::s_constructTotalCount = 0;
std::atomic<u64> Test::Object::s_destructTotalCount = 0;
std::atomic<u64> Test::Object::s_copyTotalCount = 0;
std::atomic<u64> Test::Object::s_moveTotalCount = 0;
