#include <list>
#include <doctest.h>

TEST_CASE("list")
{
    SUBCASE("empty list")
    {
        std::list<int> l;

        REQUIRE(l.size() == 0);
    }

    SUBCASE("list with elements")
    {
        std::list<int> l = {1, 2, 3};

        REQUIRE(l.size() == 3);
    }

    SUBCASE("push_back")
    {
        std::list<int> l;

        l.push_back(1);

        CHECK(l.size() == 1);
    }

    SUBCASE("push_front")
    {
        std::list<int> l;

        l.push_front(1);

        CHECK(l.size() == 1);
    }

    SUBCASE("pop_back")
    {
        std::list<int> l = {1, 2, 3};

        l.pop_back();

        CHECK(l.size() == 2);
    }

    SUBCASE("pop_front")
    {
        std::list<int> l = {1, 2, 3};

        l.pop_front();

        CHECK(l.size() == 2);
    }

    SUBCASE("insert")
    {
        std::list<int> l = {1, 2, 3};

        l.insert(l.begin(), 0);

        CHECK(l.size() == 4);
        CHECK(l.front() == 0);
    }

    SUBCASE("erase")
    {
        std::list<int> l = {1, 2, 3};

        l.erase(l.begin());

        CHECK(l.size() == 2);
        CHECK(l.front() == 2);
    }

    SUBCASE("clear")
    {
        std::list<int> l = {1, 2, 3};

        l.clear();

        CHECK(l.size() == 0);
    }

    SUBCASE("swap")
    {
        std::list<int> l1 = {1, 2, 3};
        std::list<int> l2 = {4, 5, 6};

        l1.swap(l2);

        CHECK(l1.size() == 3);
        CHECK(l2.size() == 3);
        CHECK(l1.front() == 4);
        CHECK(l2.front() == 1);
    }

    SUBCASE("splice")
    {
        std::list<int> l1 = {1, 2, 3};
        std::list<int> l2 = {4, 5, 6};

        l1.splice(l1.begin(), l2);

        CHECK(l1.size() == 6);
        CHECK(l2.size() == 0);
        CHECK(l1.front() == 4);
    }

    SUBCASE("remove")
    {
        std::list<int> l = {1, 2, 3, 2};

        l.remove(2);

        CHECK(l.size() == 2);
        CHECK(l.front() == 1);
    }

    SUBCASE("unique")
    {
        std::list<int> l = {1, 1, 2, 2, 3, 3};

        l.unique();

        CHECK(l.size() == 3);
        CHECK(l.front() == 1);
    }

    SUBCASE("sort")
    {
        std::list<int> l = {3, 2, 1};

        l.sort();

        CHECK(l.size() == 3);
        CHECK(l.front() == 1);
    }

    SUBCASE("reverse")
    {
        std::list<int> l = {1, 2, 3};

        l.reverse();

        CHECK(l.size() == 3);
        CHECK(l.front() == 3);
    }

    SUBCASE("iterator")
    {
        std::list<int> l = {1, 2, 3};

        auto it = l.begin();

        CHECK(*it == 1);
    }

    SUBCASE("const_iterator")
    {
        const std::list<int> l = {1, 2, 3};

        auto it = l.begin();

        CHECK(*it == 1);
    }

    SUBCASE("reverse_iterator")
    {
        std::list<int> l = {1, 2, 3};

        auto it = l.rbegin();

        CHECK(*it == 3);

        it++;
        CHECK(*it == 2);
    }
}
