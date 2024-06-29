#include "Shared.hpp"
#include "TestObject.hpp"

u64 TestObject::g_copyCount = 0;
u64 TestObject::g_moveCount = 0;
u64 TestObject::g_constructCount = 0;
u64 TestObject::g_destructCount = 0;
u64 TestObject::g_instanceCount = 0;
