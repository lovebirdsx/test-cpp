#include <doctest.h>
#include <string>

namespace
{
    template <typename T>
    T Add(T A, T B)
    {
        return A + B;
    }

    template <typename T>
	std::string ToString(const T& A)
	{
		return std::to_string(A);
	}

	template<>
	std::string ToString<float>(const float& A)
	{
		return std::to_string(A) + "f";
	}

    template <typename T1, typename T2 = double>
	std::string ToString2(const T1 A, const T2 B = 1.0)
	{
		return std::to_string(A) + " " + std::to_string(B);
	}

    template <typename ... TArgs>
	std::string ToString3(const TArgs& ... args)
	{
		return (... + std::to_string(args));
	}

    template <unsigned int N>
    struct Factorial
    {
        static constexpr unsigned int value = N * Factorial<N - 1>::value;
    };

    template <>
    struct Factorial<0>
    {
        static constexpr unsigned int value = 1;
    };
}

TEST_CASE("template")
{
    SUBCASE("basic")
    {
        CHECK(Add(1, 2) == 3);
        CHECK(Add(1.0, 2.0) == 3.0);
        CHECK(Add(std::string("Hello, "), std::string("World!")) == "Hello, World!");
    }

    SUBCASE("specialization")
    {
        CHECK(ToString(1) == "1");
        CHECK(ToString(1.0f) == "1.000000f");
    }

    SUBCASE("default")
    {
        CHECK(ToString2(1) == "1 1.000000");
        CHECK(ToString2(1, 2) == "1 2");
        CHECK(ToString2(1.0f) == "1.000000 1.000000");
    }

    SUBCASE("variadic")
    {
        CHECK(ToString3(1, 2, 3) == "123");
        CHECK(ToString3(1, 2, 3, 4) == "1234");
    }

    SUBCASE("recursive")
    {
        CHECK(Factorial<0>::value == 1);
        CHECK(Factorial<1>::value == 1);
        CHECK(Factorial<2>::value == 2);
        CHECK(Factorial<3>::value == 6);
        CHECK(Factorial<4>::value == 24);
        CHECK(Factorial<5>::value == 120);
    }
}
