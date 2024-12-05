#include "Shared.hpp"
#include "TestCommon.hpp"

static const char* Text = "0123456789";

char StaticFunction(int index)
{
    return Text[index];
}

class BaseClass
{
public:
    static char StaticMethod(int index)
    {
        return 'X';
    }

    virtual char Method(int index)
    {
        return 'Z';
    }

    virtual char MethodConst(int index) const
    {
        return 'Y';
    }
};

class DerivedClass : public BaseClass
{
public:
    char Method(int index) override
    {
        return Text[index];
    }

    char MethodConst(int index) const override
    {
        return Text[index] + 1;
    }
};

TestResult Common::TestFunction()
{
    // Test static functions.
    {
        Function<char(int)> function;
        TEST_FALSE(function.IsBound());

        function.Bind<&StaticFunction>();
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(6) == '6');
        TEST_TRUE(function(5) == '5');

        function.Bind(&StaticFunction);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(4) == '4');
        TEST_TRUE(function(3) == '3');

        function = &StaticFunction;
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(2) == '2');
        TEST_TRUE(function(1) == '1');

        function.Bind(nullptr);
        TEST_FALSE(function.IsBound());
    }

    // Test member functions.
    {
        Function<char(int)> function;
        TEST_FALSE(function.IsBound());

        function.Bind<&BaseClass::StaticMethod>();
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(8) == 'X');
        TEST_TRUE(function(7) == 'X');

        function.Bind(&BaseClass::StaticMethod);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(8) == 'X');
        TEST_TRUE(function(7) == 'X');

        function = &BaseClass::StaticMethod;
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(8) == 'X');
        TEST_TRUE(function(7) == 'X');

        BaseClass baseInstance;
        function.Bind<&BaseClass::Method>(&baseInstance);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(4) == 'Z');
        TEST_TRUE(function(3) == 'Z');

        const BaseClass& baseConstInstance = baseInstance;
        function.Bind<&BaseClass::MethodConst>(&baseConstInstance);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(6) == 'Y');
        TEST_TRUE(function(5) == 'Y');

        DerivedClass derivedInstance;
        function.Bind<&DerivedClass::Method>(&derivedInstance);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(2) == '2');
        TEST_TRUE(function(1) == '1');

        const DerivedClass& derivedConstInstance = derivedInstance;
        function.Bind<&DerivedClass::MethodConst>(&derivedConstInstance);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(8) == '9');
        TEST_TRUE(function(7) == '8');

        function.Bind<&BaseClass::Method>(&derivedInstance);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(6) == '6');
        TEST_TRUE(function(5) == '5');

        function.Bind<&BaseClass::MethodConst>(&derivedConstInstance);
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(4) == '5');
        TEST_TRUE(function(3) == '4');

        function.Bind(nullptr);
        TEST_FALSE(function.IsBound());
    }

    // Test lambda functions.
    {
        Function<int(int)> function;
        TEST_FALSE(function.IsBound());

        function.Bind([](int value) { return value + 1; });
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(2) == 3);
        TEST_TRUE(function(1) == 2);

        int capture = 5;
        function.Bind([&capture](int value) { return value + capture; });
        TEST_TRUE(function.IsBound());
        TEST_TRUE(function.Invoke(4) == 9);
        TEST_TRUE(function(3) == 8);

        function.Bind(nullptr);
        TEST_FALSE(function.IsBound());
    }

    return TestResult::Success;
}