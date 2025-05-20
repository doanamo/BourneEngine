#include "Shared.hpp"

TEST_DEFINE("Common.String")
{
    // #todo: Use frame allocation stats to check how many allocations were performed, now only how many are there in total.
    const Test::MemoryStats memoryStats;

    // Test default constructor
    {
        String string;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
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
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
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
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
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
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 17));

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
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

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test reserve with empty string
    {
        String string;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Reserve(15);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Reserve(16);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 17));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Reserve(20);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 21));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 20);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test reserve with string
    {
        String string("abc");
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');

        string.Reserve(15);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');

        string.Reserve(16);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 17));

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');

        string.Reserve(20);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 21));

        TEST_TRUE(string.GetLength() == 3);
        TEST_TRUE(string.GetCapacity() == 20);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "abc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "abc") == 0);
        TEST_TRUE(string[0] == 'a');
        TEST_TRUE(string[1] == 'b');
        TEST_TRUE(string[2] == 'c');
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test resize with empty string
    {
        String string;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        string.Resize(0);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Resize(15, 'b');
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "bbbbbbbbbbbbbbb") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "bbbbbbbbbbbbbbb") == 0);

        for(int i = 0; i < 15; ++i)
        {
            TEST_TRUE(string[i] == 'b');
        }

        string.Resize(0);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);

        string.Resize(16);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 17));

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "                ") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "                ") == 0);

        for(int i = 0; i < 16; ++i)
        {
            TEST_TRUE(string[i] == ' ');
        }

        string.Resize(32);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 33));

        TEST_TRUE(string.GetLength() == 32);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "                                ") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "                                ") == 0);

        for(int i = 0; i < 32; ++i)
        {
            TEST_TRUE(string[i] == ' ');
        }

        string.Resize(4, 'c');
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 33));

        TEST_TRUE(string.GetLength() == 4);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "    ") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "    ") == 0);

        for(int i = 0; i < 4; ++i)
        {
            TEST_TRUE(string[i] == ' ');
        }

        string.Resize(0, 'd');
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 33));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test resize with string
    {
        String string("hello world");
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        string.Resize(15, 'd');
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hello worlddddd") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hello worlddddd") == 0);

        string.Resize(16, 'c');
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 17));

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hello worldddddc") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hello worldddddc") == 0);

        string.Resize(32, 'a');
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 33));

        TEST_TRUE(string.GetLength() == 32);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hello worldddddcaaaaaaaaaaaaaaaa") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hello worldddddcaaaaaaaaaaaaaaaa") == 0);

        string.Resize(4, 'x');
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 33));

        TEST_TRUE(string.GetLength() == 4);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "hell") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "hell") == 0);

        string.Resize(0, 'x');
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 33));

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 32);
        TEST_TRUE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "") == 0);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test copy from default string
    {
        String input;
        String string(input);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
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
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
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
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(input.GetLength() == 15);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_FALSE(input.IsEmpty());
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
        TEST_TRUE(memoryStats.ValidateAllocations(2, sizeof(char) * 34));

        TEST_TRUE(input.GetLength() == 16);
        TEST_TRUE(input.GetCapacity() == 16);
        TEST_FALSE(input.IsEmpty());
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

        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 21));

        TEST_TRUE(input.GetLength() == 15);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_FALSE(input.IsEmpty());
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

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test copy to heap from heap
    {
        String input("0123456789abcdef");
        String string;
        string.Reserve(20);
        string = input;

        TEST_TRUE(memoryStats.ValidateAllocations(2, sizeof(char) * 38));

        TEST_TRUE(input.GetLength() == 16);
        TEST_TRUE(input.GetCapacity() == 16);
        TEST_FALSE(input.IsEmpty());
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

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test move from default string
    {
        String input;
        String string(Move(input));
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
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
        String string(Move(input));
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 0);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(string.IsEmpty());
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
        String string(Move(input));
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
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
        String string(Move(input));
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 17));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
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

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test move to heap from stack
    {
        String input("123456789abcdef");
        String string;

        string.Reserve(20);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 21));

        string = Move(input);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 15);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
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

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test move to heap from heap
    {
        String input("0123456789abcdef");
        String string;

        string.Reserve(20);
        TEST_TRUE(memoryStats.ValidateAllocations(2, sizeof(char) * 38));

        string = Move(input);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 17));

        TEST_TRUE(input.GetLength() == 0);
        TEST_TRUE(input.GetCapacity() == 15);
        TEST_TRUE(input.IsEmpty());
        TEST_TRUE(input.GetData() != nullptr);
        TEST_TRUE(strcmp(input.GetData(), "") == 0);
        TEST_TRUE(*input != nullptr);
        TEST_TRUE(*input == input.GetData());
        TEST_TRUE(strcmp(*input, "") == 0);

        TEST_TRUE(string.GetLength() == 16);
        TEST_TRUE(string.GetCapacity() == 16);
        TEST_FALSE(string.IsEmpty());
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

    // Test copy/move between different inline types
    {
        String input("123456789abcdef");

        InlineString<24> copiedLargerInline(input);
        TEST_TRUE(copiedLargerInline.GetLength() == 15);
        TEST_TRUE(copiedLargerInline.GetCapacity() == 23);
        TEST_FALSE(copiedLargerInline.IsEmpty());
        TEST_TRUE(copiedLargerInline.GetData() != nullptr);
        TEST_TRUE(strcmp(copiedLargerInline.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*copiedLargerInline != nullptr);
        TEST_TRUE(*copiedLargerInline == copiedLargerInline.GetData());
        TEST_TRUE(strcmp(*copiedLargerInline, "123456789abcdef") == 0);

        InlineString<8> copiedSmallerInline(input);
        TEST_TRUE(copiedSmallerInline.GetLength() == 15);
        TEST_TRUE(copiedSmallerInline.GetCapacity() == 15);
        TEST_FALSE(copiedSmallerInline.IsEmpty());
        TEST_TRUE(copiedSmallerInline.GetData() != nullptr);
        TEST_TRUE(strcmp(copiedSmallerInline.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*copiedSmallerInline != nullptr);
        TEST_TRUE(*copiedSmallerInline == copiedSmallerInline.GetData());
        TEST_TRUE(strcmp(*copiedSmallerInline, "123456789abcdef") == 0);

        InlineString<24> movedLargerInline(Move(copiedSmallerInline));
        TEST_TRUE(movedLargerInline.GetLength() == 15);
        TEST_TRUE(movedLargerInline.GetCapacity() == 23);
        TEST_FALSE(movedLargerInline.IsEmpty());
        TEST_TRUE(movedLargerInline.GetData() != nullptr);
        TEST_TRUE(strcmp(movedLargerInline.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*movedLargerInline != nullptr);
        TEST_TRUE(*movedLargerInline == movedLargerInline.GetData());
        TEST_TRUE(strcmp(*movedLargerInline, "123456789abcdef") == 0);

        InlineString<8> movedSmallerInline(Move(copiedLargerInline));
        TEST_TRUE(movedSmallerInline.GetLength() == 15);
        TEST_TRUE(movedSmallerInline.GetCapacity() == 15);
        TEST_FALSE(movedSmallerInline.IsEmpty());
        TEST_TRUE(movedSmallerInline.GetData() != nullptr);
        TEST_TRUE(strcmp(movedSmallerInline.GetData(), "123456789abcdef") == 0);
        TEST_TRUE(*movedSmallerInline != nullptr);
        TEST_TRUE(*movedSmallerInline == movedSmallerInline.GetData());
        TEST_TRUE(strcmp(*movedSmallerInline, "123456789abcdef") == 0);
    }

    // Test string concatenation operators
    {
        String string = String("Hello, ") + String("World!");
        TEST_TRUE(string.GetLength() == 13);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "Hello, World!") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "Hello, World!") == 0);
    }

    {
        String string = String("Hello, ") + "World!";
        TEST_TRUE(string.GetLength() == 13);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "Hello, World!") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "Hello, World!") == 0);
    }

    {
        String string = "Hello, ";
        string += String("World!");
        TEST_TRUE(string.GetLength() == 13);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "Hello, World!") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "Hello, World!") == 0);
    }

    {
        String string = "Hello, ";
        string += "World!";
        TEST_TRUE(string.GetLength() == 13);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "Hello, World!") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "Hello, World!") == 0);
    }

    // Test string self append
    {
        String string = "Hello, ";
        string += string;
        TEST_TRUE(string.GetLength() == 14);
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(strcmp(string.GetData(), "Hello, Hello, ") == 0);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(*string == string.GetData());
        TEST_TRUE(strcmp(*string, "Hello, Hello, ") == 0);
    }

    // Test string compare
    {
        String string1 = "Hello";
        String string2 = "World!";

        TEST_TRUE(string1 != string2);
        TEST_TRUE(string2 != string1);
        TEST_TRUE(string1 == string1);
        TEST_TRUE(string2 == string2);
        TEST_FALSE(string1 == string2);
        TEST_FALSE(string2 == string1);
    }

    // Test string compare with string view
    {
        String string1 = "Hello";
        StringView string2 = "World!";

        TEST_TRUE(string1 != string2);
        TEST_TRUE(string2 != string1);
        TEST_FALSE(string1 == string2);
        TEST_FALSE(string2 == string1);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test string format
    {
        String string = String::Format("Hello amazing %s!", "world");
        TEST_TRUE(string == "Hello amazing world!");
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 21));
    }

    // Test string append
    {
        String string;
        string.Append("Hello %s", "World!");
        string.Append(" ");
        string.Append("Foo %s", "Bar.");
        TEST_TRUE(string == "Hello World! Foo Bar.");
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return Test::Result::Success;
}
