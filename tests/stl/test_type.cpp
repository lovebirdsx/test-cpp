#include <doctest.h>
#include <type_traits>
#include <typeinfo>

namespace
{
    template <typename T>
    auto getReturnType() -> decltype(std::declval<T>().foo())
    {
        return 0;
    };

    class MyClass
    {
    public:
        double foo() const { return 0.0; }
    };
}

TEST_CASE("type")
{
    SUBCASE("declval")
    {
        CHECK(std::is_same_v<decltype(std::declval<MyClass>().foo()), double>);
        auto x = getReturnType<MyClass>();
        CHECK_EQ(typeid(x), typeid(double));
    }

    SUBCASE("decltype")
    {
        CHECK(std::is_same_v<decltype(MyClass().foo()), double>);
    }

    SUBCASE("is_trivial")
    {
        SUBCASE("int")
        {
            CHECK(std::is_trivial<int>::value);
        }

        SUBCASE("trivial class")
        {
            CHECK(std::is_trivial<MyClass>::value);
        }

        SUBCASE("non-trivial class")
        {
            class NonTrivialClass1 { public: NonTrivialClass1() {} };
            class NonTrivialClass2 { public: ~NonTrivialClass2() {} };
            class NonTrivialClass3 { public: virtual ~NonTrivialClass3() {} };
            class Base1 { public: virtual ~Base1() {} };
            class Derived1 : public Base1 {};
            class Base2 {};
            class Derived2 : public Base2 {};

            CHECK_FALSE(std::is_trivial<NonTrivialClass1>::value);
            CHECK_FALSE(std::is_trivial<NonTrivialClass2>::value);
            CHECK_FALSE(std::is_trivial<NonTrivialClass3>::value);
            CHECK_FALSE(std::is_trivial<Base1>::value);
            CHECK_FALSE(std::is_trivial<Derived1>::value);

            CHECK(std::is_trivial<Base2>::value);
            CHECK(std::is_trivial<Derived2>::value);
        }
    }
}