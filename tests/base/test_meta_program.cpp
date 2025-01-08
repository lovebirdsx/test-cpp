#include <doctest.h>
#include <type_traits>
#include <string>

namespace
{
    template <typename T>
    void processIntegral(T value)
    {
        static_assert(std::is_integral<T>::value, "Not an integral type");
    }

    template <typename T, bool = std::is_integral<T>::value>
    struct TypeGetter;
    
    template <typename T>
    struct TypeGetter<T, true>
    {
        static bool isIntegral() { return true; }
    };

    template <typename T>
    struct TypeGetter<T, false>
    {
        static bool isIntegral() { return false; }
    };

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, std::string>::type 
    toIntegralString(T value)
    {
        return std::to_string(value);
    }

    template <typename T>
    typename std::enable_if<!std::is_integral<T>::value, std::string>::type
    toIntegralString(T value)
    {
        return "Not an integral type";
    }

    template <typename T>
    struct isPointer : std::false_type {};

    template <typename T>
    struct isPointer<T*> : std::true_type {};

    template <typename T>
    struct isReference : std::false_type {};

    template <typename T>
    struct isReference<T&> : std::true_type {};

    template <typename T>
    bool isSizeGreaterThan4(T value, std::true_type)
    {
        return true;
    }

    template <typename T>
    bool isSizeGreaterThan4(T value, std::false_type)
    {
        return false;
    }

    template <typename T>
    bool isSizeGreaterThan4(T value)
    {
        return isSizeGreaterThan4(value, std::integral_constant<bool, (sizeof(T) > 4)>());
    }

    template <int N>
    struct factorial : std::integral_constant<int, N * factorial<N - 1>::value> {};

    template <>
    struct factorial<0> : std::integral_constant<int, 1> {};

    template <int N>
    struct febonacci : std::integral_constant<int, febonacci<N - 1>::value + febonacci<N - 2>::value> {};

    template <>
    struct febonacci<0> : std::integral_constant<int, 0> {};

    template <>
    struct febonacci<1> : std::integral_constant<int, 1> {};

    template <typename T>
    bool isConst()
    {
        if constexpr (std::is_const<T>::value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename T>
    std::string toString(T value)
    {
        if constexpr (std::is_integral<T>::value)
        {
            return std::to_string(value);
        }
        else if constexpr (std::is_floating_point<T>::value)
        {
            return std::to_string(value);
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            return value;
        }
        else if constexpr (std::is_pointer<T>::value)
        {
            return toString(std::string(value));
        }
        else
        {
            return "";
        }
    }

    template <typename... Args>
    std::string toString(Args... args)
    {
        return (... + toString(args));
    }
}

TEST_CASE("compile")
{
    SUBCASE("is_same")
    {
        CHECK(std::is_same<int, int>::value);
        CHECK_FALSE(std::is_same<int, double>::value);
    }

    SUBCASE("is_integral")
    {
        SUBCASE("check")
        {
            CHECK(std::is_integral<int>::value);
            CHECK(std::is_integral<unsigned int>::value);
            CHECK(std::is_integral<short>::value);
            CHECK(std::is_integral<unsigned short>::value);
            CHECK(std::is_integral<long>::value);
            CHECK(std::is_integral<unsigned long>::value);
            CHECK(std::is_integral<long long>::value);
            CHECK(std::is_integral<unsigned long long>::value);
            CHECK(std::is_integral<char>::value);
            CHECK(std::is_integral<unsigned char>::value);
            CHECK(std::is_integral<bool>::value);
            CHECK_FALSE(std::is_integral<float>::value);
            CHECK_FALSE(std::is_integral<double>::value);
            CHECK_FALSE(std::is_integral<long double>::value);
            CHECK_FALSE(std::is_integral<std::string>::value);
        }
        
        SUBCASE("process")
        {
            processIntegral(1);
            processIntegral(1ull);
            processIntegral('a');
            processIntegral(true);

            // compile error
            // processIntegral(1.0f);
        }

        SUBCASE("type_getter")
        {
            CHECK(TypeGetter<int>::isIntegral());
            CHECK_FALSE(TypeGetter<double>::isIntegral());
        }
    }

    SUBCASE("integral_constant")
    {
        SUBCASE("basic")
        {
            CHECK_EQ(std::integral_constant<int, 1>::value, 1);
            CHECK_EQ(std::integral_constant<unsigned int, 1>::value, 1);

            using T1 = std::integral_constant<bool, true>;
            CHECK_EQ(T1::value, true);
            CHECK_EQ(typeid(T1), typeid(std::integral_constant<bool, true>));

            using Five = std::integral_constant<int, 5>;
            CHECK_EQ(Five::value, 5);
        }

        SUBCASE("is_pointer")
        {
            CHECK_FALSE(isPointer<int>::value);
            CHECK_FALSE(isPointer<std::string>::value);
            CHECK(isPointer<int*>::value);
            CHECK(isPointer<const int*>::value);
            CHECK(isPointer<std::string*>::value);
        }

        SUBCASE("is_reference")
        {
            CHECK_FALSE(isReference<int>::value);
            CHECK_FALSE(isReference<std::string>::value);
            CHECK(isReference<int&>::value);
            CHECK(isReference<const int&>::value);
            CHECK(isReference<std::string&>::value);
        }

        SUBCASE("tag dispatching")
        {
            CHECK_FALSE(isSizeGreaterThan4(0));
            CHECK_FALSE(isSizeGreaterThan4(0.0f));

            CHECK(isSizeGreaterThan4(3.14));
            CHECK(isSizeGreaterThan4("hello"));
        }

        SUBCASE("value calculation")
        {
            CHECK_EQ(factorial<0>::value, 1);
            CHECK_EQ(factorial<3>::value, 6);
            CHECK_EQ(factorial<5>::value, 120);

            CHECK_EQ(febonacci<0>::value, 0);
            CHECK_EQ(febonacci<1>::value, 1);
            CHECK_EQ(febonacci<5>::value, 5);
        }
    }

    SUBCASE("is_floating_point")
    {
        SUBCASE("check")
        {
            CHECK_FALSE(std::is_floating_point<int>::value);
            CHECK_FALSE(std::is_floating_point<unsigned int>::value);
            CHECK(std::is_floating_point<float>::value);
            CHECK(std::is_floating_point<double>::value);
            CHECK(std::is_floating_point<long double>::value);
            CHECK_FALSE(std::is_floating_point<std::string>::value);
        }
    }
    
    SUBCASE("decay")
    {
        CHECK(std::is_same<int, std::decay<int>::type>::value);
        CHECK(std::is_same<int, std::decay<int&>::type>::value);
        CHECK(std::is_same<int, std::decay<int&&>::type>::value);
        CHECK(std::is_same<int, std::decay<const int>::type>::value);
        CHECK(std::is_same<int, std::decay<const int&>::type>::value);
        CHECK(std::is_same<int, std::decay<const int&&>::type>::value);
    }

    SUBCASE("remove_reference")
    {
        CHECK(std::is_same<int, std::remove_reference<int>::type>::value);
        CHECK(std::is_same<int, std::remove_reference<int&>::type>::value);
        CHECK(std::is_same<int, std::remove_reference<int&&>::type>::value);
        CHECK(std::is_same<const int, std::remove_reference<const int>::type>::value);
        CHECK(std::is_same<const int, std::remove_reference<const int&>::type>::value);
        CHECK(std::is_same<const int, std::remove_reference<const int&&>::type>::value);
    }

    SUBCASE("remove_cv")
    {
        CHECK(std::is_same<int, std::remove_cv<int>::type>::value);
        CHECK(std::is_same<int, std::remove_cv<const int>::type>::value);
        CHECK(std::is_same<int, std::remove_cv<volatile int>::type>::value);
        CHECK(std::is_same<int, std::remove_cv<const volatile int>::type>::value);
    }

    SUBCASE("remove_pointer")
    {
        CHECK(std::is_same<int, std::remove_pointer<int>::type>::value);
        CHECK(std::is_same<int, std::remove_pointer<int*>::type>::value);
        CHECK(std::is_same<int*, std::remove_pointer<int**>::type>::value);
        CHECK(std::is_same<int, std::remove_pointer<int* const>::type>::value);
        CHECK(std::is_same<int, std::remove_pointer<int* volatile>::type>::value);
        CHECK(std::is_same<int, std::remove_pointer<int* const volatile>::type>::value);
    }
    
    SUBCASE("void_t")
    {
        CHECK(std::is_same<void, std::void_t<int>>::value);
        CHECK(std::is_same<void, std::void_t<int, double>>::value);
        CHECK(std::is_same<void, std::void_t<int, double, std::string>>::value);
    }
    
    SUBCASE("enable_if")
    {
        SUBCASE("check")
        {
            using T1 = std::enable_if<true, int>::type;
            CHECK_EQ(typeid(T1), typeid(int));

            // compile error
            // using T2 = std::enable_if<false, int>::type;

            using T3 = std::enable_if<std::is_floating_point<float>::value, int>::type;
            CHECK_EQ(typeid(T3), typeid(int));

            // compile error
            // using T4 = std::enable_if<std::is_floating_point<int>::value, int>::type;
        }

        SUBCASE("integral")
        {
            CHECK_EQ(toIntegralString(1), "1");
            CHECK_EQ(toIntegralString(1ull), "1");
            CHECK_EQ(toIntegralString('a'), "97");
            CHECK_EQ(toIntegralString(true), "1");
            CHECK_EQ(toIntegralString(1.0f), "Not an integral type");
        }
    }

    SUBCASE("conditional")
    {
        SUBCASE("basic")
        {
            using T1 = std::conditional<true, int, double>::type;
            CHECK_EQ(typeid(T1), typeid(int));

            using T2 = std::conditional<false, int, double>::type;
            CHECK_EQ(typeid(T2), typeid(double));
        }
    }

    SUBCASE("is_base_of")
    {
        struct Base {};
        struct Derived : Base {};

        CHECK(std::is_base_of<Base, Derived>::value);
        CHECK_FALSE(std::is_base_of<Derived, Base>::value);
    }

    SUBCASE("is_convertible")
    {
        CHECK(std::is_convertible<int, double>::value);
        CHECK(std::is_convertible<double, int>::value);
    }

    SUBCASE("constexpr")
    {
        CHECK(isConst<const int>());
        CHECK_FALSE(isConst<int>());
    }

    SUBCASE("variadic templates")
    {
        class Foo { };
        struct Bar { int value; };

        CHECK_EQ(toString(1, 2), "12");
        CHECK_EQ(toString("hello", 123), "hello123");

        Foo foo;
        CHECK_EQ(toString(foo), "");

        Bar bar{ 123 };
        CHECK_EQ(toString(bar), "");
    }
}
