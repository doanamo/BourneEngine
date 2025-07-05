#include "Shared.hpp"

TEST_DEFINE("Common.StringShared", "FindIndex")
{
    String string("Hello world!");
    StringView stringView = string;

    Optional<u64> stringGoodResult = stringView.FindIndex("world");
    TEST_TRUE(stringGoodResult.HasValue());
    TEST_TRUE(stringGoodResult.GetValue() == 6);

    Optional<u64> stringViewGoodResult = stringView.FindIndex("world");
    TEST_TRUE(stringViewGoodResult.HasValue());
    TEST_TRUE(stringViewGoodResult.GetValue() == 6);

    Optional<u64> stringBadResult = stringView.FindIndex("missing");
    TEST_FALSE(stringBadResult.HasValue());

    Optional<u64> stringViewBadResult = stringView.FindIndex("missing");
    TEST_FALSE(stringViewBadResult.HasValue());

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringShared", "StartsWith")
{
    String string("Hello world!");
    TEST_TRUE(string.StartsWith("Hello"));
    TEST_FALSE(string.StartsWith("world!"));

    StringView stringView = string;
    TEST_TRUE(stringView.StartsWith("Hello"));
    TEST_FALSE(stringView.StartsWith("world!"));

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringShared", "EndsWith")
{
    String string("Hello world!");
    TEST_FALSE(string.EndsWith("Hello"));
    TEST_TRUE(string.EndsWith("world!"));

    StringView stringView = string;
    TEST_FALSE(stringView.EndsWith("Hello"));
    TEST_TRUE(stringView.EndsWith("world!"));

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringShared", "SubString")
{
    String string("Hello world!");
    TEST_TRUE(string.SubString(0, 5) == "Hello");
    TEST_TRUE(string.SubString(6, 12) == "world!");
    TEST_TRUE(string.SubString(42, 69) == "");

    TEST_TRUE(string.SubStringChopLeft(5) == "Hello");
    TEST_TRUE(string.SubStringChopRight(6) == "world!");
    TEST_TRUE(string.SubStringChopLeft(42) == "Hello world!");
    TEST_TRUE(string.SubStringChopRight(69) == "Hello world!");

    TEST_TRUE(string.SubStringTrimLeft(6) == "world!");
    TEST_TRUE(string.SubStringTrimRight(7) == "Hello");
    TEST_TRUE(string.SubStringTrimLeft(42) == "");
    TEST_TRUE(string.SubStringTrimRight(69) == "");

    TEST_TRUE(string.SubStringLeftAt(5) == "Hello");
    TEST_TRUE(string.SubStringRightAt(6) == "world!");
    TEST_TRUE(string.SubStringLeftAt(42) == "Hello world!");
    TEST_TRUE(string.SubStringRightAt(69) == "");

    StringView stringView = string;
    TEST_TRUE(stringView.SubString(0, 5) == "Hello");
    TEST_TRUE(stringView.SubString(6, 12) == "world!");
    TEST_TRUE(stringView.SubString(42, 69) == "");

    TEST_TRUE(stringView.SubStringChopLeft(5) == "Hello");
    TEST_TRUE(stringView.SubStringChopRight(6) == "world!");
    TEST_TRUE(stringView.SubStringChopLeft(42) == "Hello world!");
    TEST_TRUE(stringView.SubStringChopRight(69) == "Hello world!");

    TEST_TRUE(stringView.SubStringTrimLeft(6) == "world!");
    TEST_TRUE(stringView.SubStringTrimRight(7) == "Hello");
    TEST_TRUE(stringView.SubStringTrimLeft(42) == "");
    TEST_TRUE(stringView.SubStringTrimRight(69) == "");

    TEST_TRUE(stringView.SubStringLeftAt(5) == "Hello");
    TEST_TRUE(stringView.SubStringRightAt(6) == "world!");
    TEST_TRUE(stringView.SubStringLeftAt(42) == "Hello world!");
    TEST_TRUE(stringView.SubStringRightAt(69) == "");

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringShared", "Comparison")
{
    String string("Hello world!");
    TEST_TRUE(string == "Hello world!");
    TEST_FALSE(string == "Goodbye world!");
    TEST_FALSE(string != "Hello world!");
    TEST_TRUE(string != "Goodbye world!");

    StringView stringView = string;
    TEST_TRUE(stringView == "Hello world!");
    TEST_FALSE(stringView == "Goodbye world!");
    TEST_FALSE(stringView != "Hello world!");
    TEST_TRUE(stringView != "Goodbye world!");

    TEST_TRUE(string == stringView);
    TEST_FALSE(string != stringView);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringShared", "Accessors")
{
    const char* text = "Hello world!";

    String string(text);
    StringView stringView = string;

    TEST_TRUE(string == *stringView);
    TEST_TRUE(stringView == *string);

    u32 index = 0;
    for(char& c : string)
    {
        TEST_TRUE(c == text[index++]);
    }

    index = 0;
    for(const char& c : string)
    {
        TEST_TRUE(c == text[index++]);
    }

    index = 0;
    for(const char& c : stringView)
    {
        TEST_TRUE(c == text[index++]);
    }

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}
