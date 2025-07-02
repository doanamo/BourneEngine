#include "Shared.hpp"

TEST_DEFINE("Common.StringView", "DefaultConstructor")
{
    StringView view;
    TEST_TRUE(view.IsEmpty());
    TEST_TRUE(view.GetLength() == 0);
    TEST_TRUE(view.GetBeginPtr() != nullptr);
    TEST_TRUE(view.GetEndPtr() != nullptr);
    TEST_TRUE(view.IsNullTerminated());
    TEST_TRUE(*view != nullptr);
    TEST_TRUE(strcmp(*view, "") == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "ConstructFromData")
{
    const char* data = "Hello, World!";
    StringView view(data, std::strlen(data));

    TEST_FALSE(view.IsEmpty());
    TEST_TRUE(view.GetLength() == 13);
    TEST_TRUE(view.GetBeginPtr() == data);
    TEST_TRUE(view.GetEndPtr() != nullptr);
    TEST_TRUE(view.IsNullTerminated());
    TEST_TRUE(*view == data);
    TEST_TRUE(strcmp(*view, data) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "ConstructFromPartialText")
{
    const char* data = "Hello, World!";
    StringView view(data, std::strlen(data) - 8);

    TEST_FALSE(view.IsEmpty());
    TEST_TRUE(view.GetLength() == 5);
    TEST_TRUE(view.GetBeginPtr() == data);
    TEST_TRUE(view.GetEndPtr() == data + 5);
    TEST_FALSE(view.IsNullTerminated());
    TEST_TRUE(view.GetBeginPtr() == data);
    TEST_TRUE(memcmp(view.GetBeginPtr(), data, 5) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "CopyConstructor")
{
    const char* data = "Hello, World!";
    StringView view1(data, std::strlen(data));
    StringView view2(view1);

    TEST_FALSE(view2.IsEmpty());
    TEST_TRUE(view2.GetLength() == 13);
    TEST_TRUE(view2.GetBeginPtr() == data);
    TEST_TRUE(view2.GetEndPtr() == data + 13);
    TEST_TRUE(view2.IsNullTerminated());
    TEST_TRUE(*view2 == data);
    TEST_TRUE(strcmp(*view2, data) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "CopyFromPartialData")
{
    const char* data = "Hello, World!";
    StringView view1(data, std::strlen(data) - 8);
    StringView view2(view1);

    TEST_FALSE(view2.IsEmpty());
    TEST_TRUE(view2.GetLength() == 5);
    TEST_TRUE(view2.GetBeginPtr() == data);
    TEST_TRUE(view2.GetEndPtr() == data + 5);
    TEST_FALSE(view2.IsNullTerminated());
    TEST_TRUE(view2.GetBeginPtr() == data);
    TEST_TRUE(memcmp(view2.GetBeginPtr(), data, 5) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "MoveConstructor")
{
    const char* data = "Hello, World!";
    StringView view1(data, std::strlen(data));
    StringView view2(Move(view1));

    TEST_TRUE(view1.IsEmpty());
    TEST_TRUE(view1.GetLength() == 0);
    TEST_TRUE(view1.GetBeginPtr() != nullptr);
    TEST_TRUE(view1.GetEndPtr() != nullptr);
    TEST_TRUE(view1.IsNullTerminated());
    TEST_TRUE(*view1 != nullptr);
    TEST_TRUE(strcmp(*view1, "") == 0);

    TEST_FALSE(view2.IsEmpty());
    TEST_TRUE(view2.GetLength() == 13);
    TEST_TRUE(view2.GetBeginPtr() == data);
    TEST_TRUE(view2.GetEndPtr() == data + 13);
    TEST_TRUE(view2.IsNullTerminated());
    TEST_TRUE(*view2 == data);
    TEST_TRUE(strcmp(*view2, data) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "MoveConstructFromPartialText")
{
    const char* data = "Hello, World!";
    StringView view1(data, std::strlen(data) - 8);
    StringView view2(Move(view1));

    TEST_TRUE(view1.IsEmpty());
    TEST_TRUE(view1.GetLength() == 0);
    TEST_TRUE(view1.GetBeginPtr() != nullptr);
    TEST_TRUE(view1.GetEndPtr() != nullptr);
    TEST_TRUE(view1.IsNullTerminated());
    TEST_TRUE(*view1 != nullptr);
    TEST_TRUE(strcmp(*view1, "") == 0);

    TEST_FALSE(view2.IsEmpty());
    TEST_TRUE(view2.GetLength() == 5);
    TEST_TRUE(view2.GetBeginPtr() == data);
    TEST_TRUE(view2.GetEndPtr() == data + 5);
    TEST_FALSE(view2.IsNullTerminated());
    TEST_TRUE(view2.GetBeginPtr() == data);
    TEST_TRUE(memcmp(view2.GetBeginPtr(), data, 5) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "ToSmallString")
{
    const char* data = "Hello, World!";
    StringView view(data, std::strlen(data));
    String string = view.ToString();

    TEST_FALSE(string.IsEmpty());
    TEST_TRUE(string.GetLength() == 13);
    TEST_TRUE(string.GetData() != nullptr);
    TEST_TRUE(*string != nullptr);
    TEST_TRUE(strcmp(*string, data) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "ToLargeString")
{
    const char* data = "Hello, World and all the friends!";
    StringView view(data, std::strlen(data));
    String string = view.ToString();

    TEST_FALSE(string.IsEmpty());
    TEST_TRUE(string.GetLength() == 33);
    TEST_TRUE(string.GetData() != nullptr);
    TEST_TRUE(*string != nullptr);
    TEST_TRUE(strcmp(*string, data) == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, 34));
}

TEST_DEFINE("Common.StringView", "ToSmallStringFromPartialText")
{
    const char* data = "Hello, World!";
    StringView view(data, std::strlen(data) - 8);
    String string = view.ToString();

    TEST_FALSE(string.IsEmpty());
    TEST_TRUE(string.GetLength() == 5);
    TEST_TRUE(string.GetData() != nullptr);
    TEST_TRUE(*string != nullptr);
    TEST_TRUE(strcmp(*string, "Hello") == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "ToLargeStringFromPartialText")
{
    const char* data = "Hello, World and all the friends!";
    StringView view(data, std::strlen(data) - 13);
    String string = view.ToString();

    TEST_FALSE(string.IsEmpty());
    TEST_TRUE(string.GetLength() == 20);
    TEST_TRUE(string.GetData() != nullptr);
    TEST_TRUE(*string != nullptr);
    TEST_TRUE(strcmp(*string, "Hello, World and all") == 0);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, 21));
}

TEST_DEFINE("Common.StringView", "ComparisonOperators")
{
    StringView string1 = "Hello";
    StringView string2 = "World!";

    TEST_TRUE(string1 != string2);
    TEST_TRUE(string2 != string1);
    TEST_TRUE(string1 == string1);
    TEST_TRUE(string2 == string2);
    TEST_FALSE(string1 == string2);
    TEST_FALSE(string2 == string1);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.StringView", "CompareWithString")
{
    StringView string1 = "Hello";
    String string2 = "World!";

    TEST_TRUE(string1 != string2);
    TEST_TRUE(string2 != string1);
    TEST_FALSE(string1 == string2);
    TEST_FALSE(string2 == string1);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}
