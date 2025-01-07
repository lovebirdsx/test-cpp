#include <doctest.h>

TEST_CASE("cast")
{
    SUBCASE("static_cast")
    {
        int i = 10;
        double d = static_cast<double>(i);
        CHECK_EQ(d, 10.0);
    }

    SUBCASE("dynamic_cast")
    {
        class Base { public: virtual ~Base() = default; };
        class Derived : public Base { public: void f() {} };

        Base *b = new Derived;
        Derived *d = dynamic_cast<Derived *>(b);
        CHECK_NE(d, nullptr);
        d->f();
        delete b;
    }

    SUBCASE("const_cast")
    {
        const int i = 10;
        int *j = const_cast<int *>(&i);
        *j = 20;
        CHECK_EQ(i, 20);
    }

    SUBCASE("reinterpret_cast")
    {
        {
            int i = 10;
            char c = *reinterpret_cast<char *>(&i);
            CHECK_EQ(c, 10);
        }

        {
            int d = 0x12345678;
            char *c = reinterpret_cast<char *>(&d);
            CHECK_EQ(c[0], 0x78);
            CHECK_EQ(c[1], 0x56);
            CHECK_EQ(c[2], 0x34);
            CHECK_EQ(c[3], 0x12);
        }
    }

    SUBCASE("c-style cast")
    {
        int i = 10;
        double d = (double)i;
        CHECK_EQ(d, 10.0);
    }

    SUBCASE("c-style cast (const)")
    {
        const int i = 10;
        int j = (int)i;
        CHECK_EQ(j, 10);
    }

    SUBCASE("c-style cast (pointer)")
    {
        int i = 10;
        void *p = (void *)&i;
        CHECK_EQ(*reinterpret_cast<int *>(p), 10);
    }

    SUBCASE("c-style cast (reference)")
    {
        int i = 10;
        int &j = (int &)i;
        j = 20;
        CHECK_EQ(i, 20);
    }
}
