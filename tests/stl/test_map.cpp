#include <map>
#include <doctest.h>

TEST_CASE("map")
{
    SUBCASE("empty map")
    {
        std::map<int, int> m;

        CHECK(m.size() == 0);
    }

    SUBCASE("map with elements")
    {
        std::map<int, int> m = {{1, 2}, {3, 4}};

        CHECK(m.size() == 2);
        CHECK(m[1] == 2);
        CHECK(m[3] == 4);
    }

    SUBCASE("insert")
    {
        std::map<int, int> m = {{1, 2}, {3, 4}};

        m.insert({5, 6});
        CHECK(m.size() == 3);

        m.insert({1, 7});
        CHECK(m.size() == 3);
    }

    SUBCASE("erase")
    {
        std::map<int, int> m = {{1, 2}, {3, 4}};

        m.erase(1);

        CHECK(m.size() == 1);

        m.erase(3);
        CHECK(m.size() == 0);

        m.erase(5);
        CHECK(m.size() == 0);
    }

    SUBCASE("clear")
    {
        std::map<int, int> m = {{1, 2}, {3, 4}};

        m.clear();

        CHECK(m.size() == 0);
    }

    SUBCASE("swap")
    {
        std::map<int, int> m1 = {{1, 2}, {3, 4}};
        std::map<int, int> m2 = {{5, 6}, {7, 8}};

        m1.swap(m2);

        CHECK(m1.size() == 2);
        CHECK(m2.size() == 2);
        CHECK(m1[5] == 6);
        CHECK(m2[1] == 2);
    }

    SUBCASE("find")
    {
        std::map<int, int> m = {{1, 2}, {3, 4}};

        auto it = m.find(1);
        CHECK(it != m.end());
        CHECK(it->second == 2);

        it = m.find(5);
        CHECK(it == m.end());
    }

    SUBCASE("count")
    {
        std::map<int, int> m = {{1, 2}, {3, 4}};

        CHECK(m.count(1) == 1);
        CHECK(m.count(5) == 0);
    }
}
