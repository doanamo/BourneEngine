#include "Shared.hpp"
#include "TestObject.hpp"

u64 TestObject::s_copyCount = 0;
u64 TestObject::s_moveCount = 0;
u64 TestObject::s_constructCount = 0;
u64 TestObject::s_destructCount = 0;
u64 TestObject::s_instanceCount = 0;
