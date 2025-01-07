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
    toString(T value)
    {
        return std::to_string(value);
    }

    template <typename T>
    typename std::enable_if<!std::is_integral<T>::value, std::string>::type
    toString(T value)
    {
        return "Not an integral type";
    }
}

TEST_CASE("compile")
{
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

    SUBCASE("enable_if")
    {
        SUBCASE("integral")
        {
            CHECK_EQ(toString(1), "1");
            CHECK_EQ(toString(1ull), "1");
            CHECK_EQ(toString('a'), "97");
            CHECK_EQ(toString(true), "1");
            CHECK_EQ(toString(1.0f), "Not an integral type");
        }
    }
}
