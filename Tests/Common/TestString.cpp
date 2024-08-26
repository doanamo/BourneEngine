#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestString()
{
    LOG_INFO("Running Common::TestString...");

#ifdef ENABLE_MEMORY_STATS
    u64 baseAllocationCount = Memory::Stats::Get().GetAllocationCount();
    u64 baseAllocatedBytes = Memory::Stats::Get().GetAllocatedUsableBytes();
#endif

    // Test default constructor
    {
        String string;

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == String::MaxSmallLength);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        const String& constString = string;
        TEST_TRUE(constString.GetData() != nullptr);
        TEST_TRUE(constString.GetData() == string.GetData());
        TEST_TRUE(strcmp(constString.GetData(), "") == 0);
        TEST_TRUE(*constString != nullptr);
        TEST_TRUE(*constString == *string);
        TEST_TRUE(*constString == constString.GetData());
        TEST_TRUE(strcmp(*constString, "") == 0);
    }

    // Test empty text constructor
    {
        String string("");

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == String::MaxSmallLength);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        const String& constString = string;
        TEST_TRUE(constString.GetData() != nullptr);
        TEST_TRUE(constString.GetData() == string.GetData());
        TEST_TRUE(strcmp(constString.GetData(), "") == 0);
        TEST_TRUE(*constString != nullptr);
        TEST_TRUE(*constString == *string);
        TEST_TRUE(*constString == constString.GetData());
        TEST_TRUE(strcmp(*constString, "") == 0);
    }

    // Test stack constructor
    {
        String string("123456789abcdef");

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "123456789abcdef") == 0);
        TEST_TRUE(string[0] == '1');
        TEST_TRUE(string[1] == '2');
        TEST_TRUE(string[2] == '3');
        TEST_TRUE(string[3] == '4');
        TEST_TRUE(string[4] == '5');
        TEST_TRUE(string[5] == '6');
        TEST_TRUE(string[6] == '7');
        TEST_TRUE(string[7] == '8');
        TEST_TRUE(string[8] == '9');
        TEST_TRUE(string[9] == 'a');
        TEST_TRUE(string[10] == 'b');
        TEST_TRUE(string[11] == 'c');
        TEST_TRUE(string[12] == 'd');
        TEST_TRUE(string[13] == 'e');
        TEST_TRUE(string[14] == 'f');
    }

    // Test heap constructor
    {
        String string("0123456789abcdef");

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
#endif

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "0123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "0123456789abcdef") == 0);
        TEST_TRUE(string[0] == '0');
        TEST_TRUE(string[1] == '1');
        TEST_TRUE(string[2] == '2');
        TEST_TRUE(string[3] == '3');
        TEST_TRUE(string[4] == '4');
        TEST_TRUE(string[5] == '5');
        TEST_TRUE(string[6] == '6');
        TEST_TRUE(string[7] == '7');
        TEST_TRUE(string[8] == '8');
        TEST_TRUE(string[9] == '9');
        TEST_TRUE(string[10] == 'a');
        TEST_TRUE(string[11] == 'b');
        TEST_TRUE(string[12] == 'c');
        TEST_TRUE(string[13] == 'd');
        TEST_TRUE(string[14] == 'e');
        TEST_TRUE(string[15] == 'f');
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test reserve with empty string
    {
        String string;

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Reserve(15);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Reserve(16);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
#endif

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_TRUE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Reserve(20);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
#endif

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 20);
        TEST_TRUE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test reserve with string
    {
        String string("abc");

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');

        string.Reserve(15);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');

        string.Reserve(16);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
#endif

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');

        string.Reserve(20);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
#endif

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 20);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test resize with empty string
    {
        String string;

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        string.Resize(0);
        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        string.Resize(15, 'b');
        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "bbbbbbbbbbbbbbb") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "bbbbbbbbbbbbbbb") == 0);

        for(int i = 0; i < 15; ++i)
        {
            TEST_TRUE(string[i] == 'b');
        }

 #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        string.Resize(0);
        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        string.Resize(16);
        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        for(int i = 0; i < 16; ++i)
        {
            TEST_TRUE(string[i] == '\0');
        }

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
#endif

        string.Resize(32);
        TEST_TRUE(string.GetLength() == 32);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        for(int i = 0; i < 32; ++i)
        {
            TEST_TRUE(string[i] == '\0');
        }

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 33);
#endif

        string.Resize(4, 'c');
        TEST_TRUE(string.GetLength() == 4);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        for(int i = 0; i < 4; ++i)
        {
            TEST_TRUE(string[i] == '\0');
        }

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 33);
#endif

        string.Resize(0, 'd');
        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_TRUE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 33);
#endif
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test resize with string
    {
        String string("hello world");

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif

        string.Resize(15, 'd');
        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hello worlddddd") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hello worlddddd") == 0);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif

        string.Resize(16, 'c');
        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hello worldddddc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hello worldddddc") == 0);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
    #endif

        string.Resize(32, 'a');
        TEST_TRUE(string.GetLength() == 32);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hello worldddddcaaaaaaaaaaaaaaaa") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hello worldddddcaaaaaaaaaaaaaaaa") == 0);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 33);
    #endif

        string.Resize(4, 'x');
        TEST_TRUE(string.GetLength() == 4);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hell") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hell") == 0);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 33);
    #endif

        string.Resize(0, 'x');
        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_TRUE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 33);
    #endif
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test copy from default string
    {
        String input;
        String string(input);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy from empty string
    {
        String input("");
        String string(input);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy to stack from stack
    {
        String input("123456789abcdef");
        String string(input);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(input.GetLength() == 15);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_FALSE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "123456789abcdef") == 0);
        TEST_TRUE(input[0] == '1');
        TEST_TRUE(input[1] == '2');
        TEST_TRUE(input[2] == '3');
        TEST_TRUE(input[3] == '4');
        TEST_TRUE(input[4] == '5');
        TEST_TRUE(input[5] == '6');
        TEST_TRUE(input[6] == '7');
        TEST_TRUE(input[7] == '8');
        TEST_TRUE(input[8] == '9');
        TEST_TRUE(input[9] == 'a');
        TEST_TRUE(input[10] == 'b');
        TEST_TRUE(input[11] == 'c');
        TEST_TRUE(input[12] == 'd');
        TEST_TRUE(input[13] == 'e');
        TEST_TRUE(input[14] == 'f');

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "123456789abcdef") == 0);
        TEST_TRUE(string[0] == '1');
        TEST_TRUE(string[1] == '2');
        TEST_TRUE(string[2] == '3');
        TEST_TRUE(string[3] == '4');
        TEST_TRUE(string[4] == '5');
        TEST_TRUE(string[5] == '6');
        TEST_TRUE(string[6] == '7');
        TEST_TRUE(string[7] == '8');
        TEST_TRUE(string[8] == '9');
        TEST_TRUE(string[9] == 'a');
        TEST_TRUE(string[10] == 'b');
        TEST_TRUE(string[11] == 'c');
        TEST_TRUE(string[12] == 'd');
        TEST_TRUE(string[13] == 'e');
        TEST_TRUE(string[14] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy to stack from heap
    {
        String input("0123456789abcdef");
        String string(input);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 2);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 34);
#endif

        TEST_TRUE(input.GetLength() == 16);
        TEST_TRUE(input.GetCapacity() == 16);
        TEST_FALSE(input.IsEmpty());
        TEST_FALSE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "0123456789abcdef") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "0123456789abcdef") == 0);
        TEST_TRUE(input[0] == '0');
        TEST_TRUE(input[1] == '1');
        TEST_TRUE(input[2] == '2');
        TEST_TRUE(input[3] == '3');
        TEST_TRUE(input[4] == '4');
        TEST_TRUE(input[5] == '5');
        TEST_TRUE(input[6] == '6');
        TEST_TRUE(input[7] == '7');
        TEST_TRUE(input[8] == '8');
        TEST_TRUE(input[9] == '9');
        TEST_TRUE(input[10] == 'a');
        TEST_TRUE(input[11] == 'b');
        TEST_TRUE(input[12] == 'c');
        TEST_TRUE(input[13] == 'd');
        TEST_TRUE(input[14] == 'e');
        TEST_TRUE(input[15] == 'f');

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "0123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "0123456789abcdef") == 0);
        TEST_TRUE(string[0] == '0');
        TEST_TRUE(string[1] == '1');
        TEST_TRUE(string[2] == '2');
        TEST_TRUE(string[3] == '3');
        TEST_TRUE(string[4] == '4');
        TEST_TRUE(string[5] == '5');
        TEST_TRUE(string[6] == '6');
        TEST_TRUE(string[7] == '7');
        TEST_TRUE(string[8] == '8');
        TEST_TRUE(string[9] == '9');
        TEST_TRUE(string[10] == 'a');
        TEST_TRUE(string[11] == 'b');
        TEST_TRUE(string[12] == 'c');
        TEST_TRUE(string[13] == 'd');
        TEST_TRUE(string[14] == 'e');
        TEST_TRUE(string[15] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy to heap from stack
    {
        String input("123456789abcdef");
        String string;
        string.Reserve(20);
        string = input;

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
#endif

        TEST_TRUE(input.GetLength() == 15);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_FALSE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "123456789abcdef") == 0);
        TEST_TRUE(input[0] == '1');
        TEST_TRUE(input[1] == '2');
        TEST_TRUE(input[2] == '3');
        TEST_TRUE(input[3] == '4');
        TEST_TRUE(input[4] == '5');
        TEST_TRUE(input[5] == '6');
        TEST_TRUE(input[6] == '7');
        TEST_TRUE(input[7] == '8');
        TEST_TRUE(input[8] == '9');
        TEST_TRUE(input[9] == 'a');
        TEST_TRUE(input[10] == 'b');
        TEST_TRUE(input[11] == 'c');
        TEST_TRUE(input[12] == 'd');
        TEST_TRUE(input[13] == 'e');
        TEST_TRUE(input[14] == 'f');

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 20);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "123456789abcdef") == 0);
        TEST_TRUE(string[0] == '1');
        TEST_TRUE(string[1] == '2');
        TEST_TRUE(string[2] == '3');
        TEST_TRUE(string[3] == '4');
        TEST_TRUE(string[4] == '5');
        TEST_TRUE(string[5] == '6');
        TEST_TRUE(string[6] == '7');
        TEST_TRUE(string[7] == '8');
        TEST_TRUE(string[8] == '9');
        TEST_TRUE(string[9] == 'a');
        TEST_TRUE(string[10] == 'b');
        TEST_TRUE(string[11] == 'c');
        TEST_TRUE(string[12] == 'd');
        TEST_TRUE(string[13] == 'e');
        TEST_TRUE(string[14] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test copy to heap from heap
    {
        String input("0123456789abcdef");
        String string;
        string.Reserve(20);
        string = input;

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 2);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 38);
#endif

        TEST_TRUE(input.GetLength() == 16);
        TEST_TRUE(input.GetCapacity() == 16);
        TEST_FALSE(input.IsEmpty());
        TEST_FALSE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "0123456789abcdef") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "0123456789abcdef") == 0);
        TEST_TRUE(input[0] == '0');
        TEST_TRUE(input[1] == '1');
        TEST_TRUE(input[2] == '2');
        TEST_TRUE(input[3] == '3');
        TEST_TRUE(input[4] == '4');
        TEST_TRUE(input[5] == '5');
        TEST_TRUE(input[6] == '6');
        TEST_TRUE(input[7] == '7');
        TEST_TRUE(input[8] == '8');
        TEST_TRUE(input[9] == '9');
        TEST_TRUE(input[10] == 'a');
        TEST_TRUE(input[11] == 'b');
        TEST_TRUE(input[12] == 'c');
        TEST_TRUE(input[13] == 'd');
        TEST_TRUE(input[14] == 'e');
        TEST_TRUE(input[15] == 'f');

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 20);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "0123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "0123456789abcdef") == 0);
        TEST_TRUE(string[0] == '0');
        TEST_TRUE(string[1] == '1');
        TEST_TRUE(string[2] == '2');
        TEST_TRUE(string[3] == '3');
        TEST_TRUE(string[4] == '4');
        TEST_TRUE(string[5] == '5');
        TEST_TRUE(string[6] == '6');
        TEST_TRUE(string[7] == '7');
        TEST_TRUE(string[8] == '8');
        TEST_TRUE(string[9] == '9');
        TEST_TRUE(string[10] == 'a');
        TEST_TRUE(string[11] == 'b');
        TEST_TRUE(string[12] == 'c');
        TEST_TRUE(string[13] == 'd');
        TEST_TRUE(string[14] == 'e');
        TEST_TRUE(string[15] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test move from default string
    {
        String input;
        String string(std::move(input));

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test move from empty string
    {
        String input("");
        String string(std::move(input));

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test move to stack from stack
    {
        String input("123456789abcdef");
        String string(std::move(input));

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "123456789abcdef") == 0);
        TEST_TRUE(string[0] == '1');
        TEST_TRUE(string[1] == '2');
        TEST_TRUE(string[2] == '3');
        TEST_TRUE(string[3] == '4');
        TEST_TRUE(string[4] == '5');
        TEST_TRUE(string[5] == '6');
        TEST_TRUE(string[6] == '7');
        TEST_TRUE(string[7] == '8');
        TEST_TRUE(string[8] == '9');
        TEST_TRUE(string[9] == 'a');
        TEST_TRUE(string[10] == 'b');
        TEST_TRUE(string[11] == 'c');
        TEST_TRUE(string[12] == 'd');
        TEST_TRUE(string[13] == 'e');
        TEST_TRUE(string[14] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test move to stack from heap
    {
        String input("0123456789abcdef");
        String string(std::move(input));

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "0123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "0123456789abcdef") == 0);
        TEST_TRUE(string[0] == '0');
        TEST_TRUE(string[1] == '1');
        TEST_TRUE(string[2] == '2');
        TEST_TRUE(string[3] == '3');
        TEST_TRUE(string[4] == '4');
        TEST_TRUE(string[5] == '5');
        TEST_TRUE(string[6] == '6');
        TEST_TRUE(string[7] == '7');
        TEST_TRUE(string[8] == '8');
        TEST_TRUE(string[9] == '9');
        TEST_TRUE(string[10] == 'a');
        TEST_TRUE(string[11] == 'b');
        TEST_TRUE(string[12] == 'c');
        TEST_TRUE(string[13] == 'd');
        TEST_TRUE(string[14] == 'e');
        TEST_TRUE(string[15] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test move to heap from stack
    {
        String input("123456789abcdef");
        String string;
        string.Reserve(20);
        string = std::move(input);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 20);
        TEST_TRUE(input.IsEmpty());
        TEST_FALSE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "123456789abcdef") == 0);
        TEST_TRUE(string[0] == '1');
        TEST_TRUE(string[1] == '2');
        TEST_TRUE(string[2] == '3');
        TEST_TRUE(string[3] == '4');
        TEST_TRUE(string[4] == '5');
        TEST_TRUE(string[5] == '6');
        TEST_TRUE(string[6] == '7');
        TEST_TRUE(string[7] == '8');
        TEST_TRUE(string[8] == '9');
        TEST_TRUE(string[9] == 'a');
        TEST_TRUE(string[10] == 'b');
        TEST_TRUE(string[11] == 'c');
        TEST_TRUE(string[12] == 'd');
        TEST_TRUE(string[13] == 'e');
        TEST_TRUE(string[14] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test move to heap from heap
    {
        String input("0123456789abcdef");
        String string;
        string.Reserve(20);
        string = std::move(input);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 2);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 38);
#endif

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 20);
        TEST_TRUE(input.IsEmpty());
        TEST_FALSE(input.IsSmall());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "0123456789abcdef") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "0123456789abcdef") == 0);
        TEST_TRUE(string[0] == '0');
        TEST_TRUE(string[1] == '1');
        TEST_TRUE(string[2] == '2');
        TEST_TRUE(string[3] == '3');
        TEST_TRUE(string[4] == '4');
        TEST_TRUE(string[5] == '5');
        TEST_TRUE(string[6] == '6');
        TEST_TRUE(string[7] == '7');
        TEST_TRUE(string[8] == '8');
        TEST_TRUE(string[9] == '9');
        TEST_TRUE(string[10] == 'a');
        TEST_TRUE(string[11] == 'b');
        TEST_TRUE(string[12] == 'c');
        TEST_TRUE(string[13] == 'd');
        TEST_TRUE(string[14] == 'e');
        TEST_TRUE(string[15] == 'f');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifdef ENABLE_MEMORY_STATS
    TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    return TestResult::Success;
}
