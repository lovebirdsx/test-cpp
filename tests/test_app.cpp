#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "util.h"


TEST_CASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

TEST_CASE("vectors can be sized and resized") {
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SUBCASE("adding to the vector increases its size") {
        v.push_back(1);

        CHECK(v.size() == 6);
        CHECK(v.capacity() >= 6);
    }

    SUBCASE("reserving increases just the capacity") {
        v.reserve(6);

        CHECK(v.size() == 5);
        CHECK(v.capacity() >= 6);
    }
}
