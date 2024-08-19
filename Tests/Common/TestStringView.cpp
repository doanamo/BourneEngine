#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestStringView()
{
    LOG_INFO("Running Common::TestStringView...");

    // Test empty constructor
    {
        StringView view;

        TEST_TRUE(view.IsEmpty());
        TEST_TRUE(view.GetLength() == 0);
        TEST_TRUE(view.GetData() != nullptr);
        TEST_TRUE(*view != nullptr);
        TEST_TRUE(strcmp(*view, "") == 0);
    }

    // Test constructor from data and length
    {
        const char* data = "Hello, World!";
        StringView view(data, strlen(data));

        TEST_FALSE(view.IsEmpty());
        TEST_TRUE(view.GetLength() == 13);
        TEST_TRUE(view.GetData() == data);
        TEST_TRUE(*view == data);
        TEST_TRUE(strcmp(*view, data) == 0);
    }

    // Test constructor from data and incomplete length
    {
        const char* data = "Hello, World!";
        StringView view(data, strlen(data) - 8);

        TEST_FALSE(view.IsEmpty());
        TEST_TRUE(view.GetLength() == 5);
        TEST_TRUE(view.GetData() == data);
        TEST_TRUE(*view == data);
        TEST_TRUE(strcmp(*view, data) == 0);
    }

    // Test copy constructor
    {
        const char* data = "Hello, World!";
        StringView view1(data, strlen(data));
        StringView view2(view1);

        TEST_FALSE(view2.IsEmpty());
        TEST_TRUE(view2.GetLength() == 13);
        TEST_TRUE(view2.GetData() == data);
        TEST_TRUE(*view2 == data);
        TEST_TRUE(strcmp(*view2, data) == 0);
    }

    // Test copy constructor with incomplete length
    {
        const char* data = "Hello, World!";
        StringView view1(data, strlen(data) - 8);
        StringView view2(view1);

        TEST_FALSE(view2.IsEmpty());
        TEST_TRUE(view2.GetLength() == 5);
        TEST_TRUE(view2.GetData() == data);
        TEST_TRUE(*view2 == data);
        TEST_TRUE(strcmp(*view2, data) == 0);
    }

    // Test move constructor
    {
        const char* data = "Hello, World!";
        StringView view1(data, strlen(data));
        StringView view2(std::move(view1));

        TEST_TRUE(view1.IsEmpty());
        TEST_TRUE(view1.GetLength() == 0);
        TEST_TRUE(view1.GetData() != nullptr);
        TEST_TRUE(*view1 != nullptr);
        TEST_TRUE(strcmp(*view1, "") == 0);

        TEST_FALSE(view2.IsEmpty());
        TEST_TRUE(view2.GetLength() == 13);
        TEST_TRUE(view2.GetData() == data);
        TEST_TRUE(*view2 == data);
        TEST_TRUE(strcmp(*view2, data) == 0);
    }

    // Test move constructor with incomplete length
    {
        const char* data = "Hello, World!";
        StringView view1(data, strlen(data) - 8);
        StringView view2(std::move(view1));

        TEST_TRUE(view1.IsEmpty());
        TEST_TRUE(view1.GetLength() == 0);
        TEST_TRUE(view1.GetData() != nullptr);
        TEST_TRUE(*view1 != nullptr);
        TEST_TRUE(strcmp(*view1, "") == 0);

        TEST_FALSE(view2.IsEmpty());
        TEST_TRUE(view2.GetLength() == 5);
        TEST_TRUE(view2.GetData() == data);
        TEST_TRUE(*view2 == data);
        TEST_TRUE(strcmp(*view2, data) == 0);
    }

    // Test conversion to small string
    {
        const char* data = "Hello, World!";
        StringView view(data, strlen(data));
        String string = view.ToString();

        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetLength() == 13);
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(strcmp(*string, data) == 0);
    }

    // Test conversion to large string
    {
        const char* data = "Hello, World and all the friends!";
        StringView view(data, strlen(data));
        String string = view.ToString();

        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetLength() == 33);
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(strcmp(*string, data) == 0);
    }

    // Test conversion to small string from incomplete length
    {
        const char* data = "Hello, World!";
        StringView view(data, strlen(data) - 8);
        String string = view.ToString();

        TEST_FALSE(string.IsEmpty());
        TEST_TRUE(string.IsSmall());
        TEST_TRUE(string.GetLength() == 5);
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(strcmp(*string, "Hello") == 0);
    }

    // Test conversion to large string from incomplete length
    {
        const char* data = "Hello, World and all the friends!";
        StringView view(data, strlen(data) - 13);
        String string = view.ToString();

        TEST_FALSE(string.IsEmpty());
        TEST_FALSE(string.IsSmall());
        TEST_TRUE(string.GetLength() == 20);
        TEST_TRUE(string.GetData() != nullptr);
        TEST_TRUE(*string != nullptr);
        TEST_TRUE(strcmp(*string, "Hello, World and all") == 0);
    }

    return TestResult::Success;
}
