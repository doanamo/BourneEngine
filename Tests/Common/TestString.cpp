#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestString()
{
#ifndef CONFIG_RELEASE
    u64 baseAllocationCount = Memory::DefaultAllocator::GetAllocationCount();
    u64 baseAllocatedBytes = Memory::DefaultAllocator::GetAllocatedUsableBytes();
#endif

    // Test default constructor
    {
        String string;

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(string[0] == '\0');

        const String<>& constString = string;
        TEST_TRUE(constString.GetData() != nullptr);
        TEST_TRUE(constString.GetData() == string.GetData());
        TEST_TRUE(strcmp(constString.GetData(), "") == 0);
        TEST_TRUE(*constString != nullptr);
        TEST_TRUE(*constString == *string);
        TEST_TRUE(*constString == constString.GetData());
        TEST_TRUE(strcmp(*constString, "") == 0);
        TEST_TRUE(constString[0] == '\0');
    }

    // Test empty text constructor
    {
        String string("");

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(string[0] == '\0');

        const String<>& constString = string;
        TEST_TRUE(constString.GetData() != nullptr);
        TEST_TRUE(constString.GetData() == string.GetData());
        TEST_TRUE(strcmp(constString.GetData(), "") == 0);
        TEST_TRUE(*constString != nullptr);
        TEST_TRUE(*constString == *string);
        TEST_TRUE(*constString == constString.GetData());
        TEST_TRUE(strcmp(*constString, "") == 0);
        TEST_TRUE(constString[0] == '\0');
    }

    // Test stack constructor
    {
        String string("123456789abcdef");

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(string[15] == '\0');
    }

    // Test heap constructor
    {
        String string("0123456789abcdef");

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
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
        TEST_TRUE(string[16] == '\0');
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test reserve with empty string
    {
        String string;

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(string[0] == '\0');

        string.Reserve(15);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(string[0] == '\0');

        string.Reserve(16);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
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
        TEST_TRUE(string[0] == '\0');

        string.Reserve(20);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
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
        TEST_TRUE(string[0] == '\0');
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test reserve with string
    {
        String string("abc");

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(string[3] == '\0');

        string.Reserve(15);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(string[3] == '\0');

        string.Reserve(16);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
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
        TEST_TRUE(string[3] == '\0');

        string.Reserve(20);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
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
        TEST_TRUE(string[3] == '\0');
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test copy from default string
    {
        String input;
        String string(input);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(input[0] == '\0');

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
        TEST_TRUE(string[0] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy from empty string
    {
        String input("");
        String string(input);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(input[0] == '\0');

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
        TEST_TRUE(string[0] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy to stack from stack
    {
        String input("123456789abcdef");
        String string(input);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(input[15] == '\0');

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
        TEST_TRUE(string[15] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy to stack from heap
    {
        String input("0123456789abcdef");
        String string(input);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 2);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 34);
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
        TEST_TRUE(input[16] == '\0');

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
        TEST_TRUE(string[16] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test copy to heap from stack
    {
        String input("123456789abcdef");
        String string;
        string.Reserve(20);
        string = input;

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
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
        TEST_TRUE(input[15] == '\0');

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
        TEST_TRUE(string[15] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test copy to heap from heap
    {
        String input("0123456789abcdef");
        String string;
        string.Reserve(20);
        string = input;

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 2);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 38);
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
        TEST_TRUE(input[16] == '\0');

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
        TEST_TRUE(string[16] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test move from default string
    {
        String input;
        String string(std::move(input));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(input[0] == '\0');

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
        TEST_TRUE(string[0] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test move from empty string
    {
        String input("");
        String string(std::move(input));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(input[0] == '\0');

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
        TEST_TRUE(string[0] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test move to stack from stack
    {
        String input("123456789abcdef");
        String string(std::move(input));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
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
        TEST_TRUE(input[0] == '\0');

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
        TEST_TRUE(string[15] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

    // Test move to stack from heap
    {
        String input("0123456789abcdef");
        String string(std::move(input));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 17);
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
        TEST_TRUE(input[0] == '\0');

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
        TEST_TRUE(string[16] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test move to heap from stack
    {
        String input("123456789abcdef");
        String string;
        string.Reserve(20);
        string = std::move(input);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 21);
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
        TEST_TRUE(input[0] == '\0');

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
        TEST_TRUE(string[15] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    // Test move to heap from heap
    {
        String input("0123456789abcdef");
        String string;
        string.Reserve(20);
        string = std::move(input);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount + 2);
        TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(char) * 38);
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
        TEST_TRUE(input[0] == '\0');

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
        TEST_TRUE(string[16] == '\0');

        TEST_TRUE(input.GetData() != string.GetData());
        TEST_TRUE(*input != *string);
    }

#ifndef CONFIG_RELEASE
    TEST_TRUE(Memory::DefaultAllocator::GetAllocationCount() == baseAllocationCount);
    TEST_TRUE(Memory::DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif

    return TestResult::Success;
}
