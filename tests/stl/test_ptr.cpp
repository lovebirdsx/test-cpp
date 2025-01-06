#include <string>
#include <doctest.h>
#include <memory>

TEST_CASE("ptr")
{
    SUBCASE("unique_ptr")
    {
        std::unique_ptr<int> p = std::make_unique<int>(42);

        SUBCASE("get")
        {
            CHECK(*p == 42);

            CHECK(p.get() != nullptr);
        }

        SUBCASE("reset")
        {
            p.reset();
            CHECK(p == nullptr);
            CHECK(p.get() == nullptr);
        }

        SUBCASE("release")
        {
            int *ptr = p.release();
            CHECK(ptr != nullptr);
            CHECK(*ptr == 42);

            delete ptr;
        }
    }

    SUBCASE("shared_ptr")
    {
        std::shared_ptr<int> p = std::make_shared<int>(42);

        SUBCASE("get")
        {
            CHECK(*p == 42);

            CHECK(p.get() != nullptr);
        }

        SUBCASE("reset")
        {
            p.reset();
            CHECK(p.get() == nullptr);
        }

        SUBCASE("use_count")
        {
            CHECK(p.use_count() == 1);

            std::shared_ptr<int> p2 = p;
            CHECK(p.use_count() == 2);

            p2.reset();
            CHECK(p.use_count() == 1);
        }

        SUBCASE("make_shared")
        {
            auto p2 = std::make_shared<std::string>("hello");
            CHECK(*p2 == "hello");

            auto p3 = p2;
            CHECK(p2.use_count() == 2);

            p2.reset();
            CHECK(p3.use_count() == 1);
            CHECK(*p3 == "hello");
        }

        SUBCASE("shared_from_this")
        {
            struct A : std::enable_shared_from_this<A>
            {
                std::shared_ptr<A> get()
                {
                    return shared_from_this();
                }
            };

            std::shared_ptr<A> a = std::make_shared<A>();
            std::shared_ptr<A> b = a->get();
            
            CHECK(a.get() == b.get());
            CHECK(a.use_count() == 2);

            std::shared_ptr<A> c = std::make_shared<A>();
            CHECK(c.use_count() == 1);
        }
    }

    SUBCASE("weak_ptr")
    {
        std::shared_ptr<int> p = std::make_shared<int>(42);
        std::weak_ptr<int> w = p;

        SUBCASE("lock")
        {
            auto p2 = w.lock();
            CHECK(p2.get() != nullptr);
            CHECK(*p2 == 42);
            CHECK(p.use_count() == 2);

            p.reset();
            CHECK(p2.use_count() == 1);
            CHECK(p2.get() != nullptr);

            p2.reset();
            CHECK(w.expired());
            CHECK(w.lock().get() == nullptr);
            CHECK(w.use_count() == 0);
        }

        SUBCASE("expired")
        {
            CHECK(!w.expired());

            p.reset();
            CHECK(w.expired());
        }
    }
}