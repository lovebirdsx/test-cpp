#include <string>
#include <doctest.h>
#include <string_view>

TEST_CASE("string")
{
    SUBCASE("simple")
    {
        std::string s = "hello";
        CHECK_EQ(s.size(), 5);

        std::string_view sv = s;
        CHECK_EQ(sv.size(), 5);
    }
}