#include <doctest.h>
#include <type_traits>
#include <typeinfo>

namespace
{
    template <typename T>
    auto getReturnType() -> decltype(std::declval<T>().foo()) {};
    
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
}