#include <doctest.h>
#include <iostream>

TEST_CASE("type")
{
    SUBCASE("decltype")
    {
        SUBCASE("primitive type")
        {
            int a = 0;
            decltype(a) b = 1;
            CHECK_EQ(typeid(b), typeid(int));
        }

        SUBCASE("expression")
        {
            double x  = 3.14;
            int y = 42;
            decltype(x + y) z = 0;
            CHECK_EQ(typeid(z), typeid(double));
        }
    }

    SUBCASE("typeid")
    {
        SUBCASE("primitive type")
        {
            int a = 0;
            CHECK_EQ(typeid(int), typeid(int));
            CHECK_EQ(typeid(a), typeid(int));
        }

        SUBCASE("custom class")
        {
            class A {};

            A a;
            CHECK_EQ(typeid(A), typeid(A));
            CHECK_EQ(typeid(a), typeid(A));
        }

        SUBCASE("pointer")
        {
            int a = 0;
            int *pa = &a;
            CHECK_EQ(typeid(int *), typeid(int *));
            CHECK_EQ(typeid(pa), typeid(int *));
            
            char *pc = nullptr;
            CHECK_EQ(typeid(char *), typeid(char *));
            CHECK_EQ(typeid(pc), typeid(char *));
            CHECK_NE(typeid(char *), typeid(int *));
        }

        SUBCASE("reference")
        {
            int a = 0;
            int &ra = a;
            CHECK_EQ(typeid(int &), typeid(int &));
            CHECK_EQ(typeid(const int &), typeid(int &));
            CHECK_EQ(typeid(ra), typeid(int &));
        }

        SUBCASE("const")
        {
            const int a = 0;
            CHECK_EQ(typeid(const int), typeid(const int));
            CHECK_EQ(typeid(a), typeid(const int));
            CHECK_EQ(typeid(int), typeid(const int));
        }

        SUBCASE("array")
        {
            int a[10];
            CHECK_EQ(typeid(int[10]), typeid(int[10]));
            CHECK_EQ(typeid(a), typeid(int[10]));

            int b[5];
            CHECK_NE(typeid(a), typeid(b));
        }

        SUBCASE("function")
        {
            void (*f)() = []() {};
            CHECK_EQ(typeid(decltype(f)), typeid(decltype(f)));
            CHECK_EQ(typeid(f), typeid(decltype(f)));
            CHECK_EQ(typeid(f), typeid(void (*)()));
        }

        SUBCASE("derived class")
        {
            class Base {};
            class Derived1: public Base {};
            class Derived2: public Base {};

            Base *d1 = new Derived1;
            Base *d2 = new Derived2;

            CHECK_EQ(typeid(d1), typeid(Base *));
            CHECK_EQ(typeid(*d1), typeid(Base));
        }

        SUBCASE("nullptr")
        {
            CHECK_EQ(typeid(nullptr), typeid(std::nullptr_t));

            int *p = nullptr;
            CHECK_EQ(typeid(*p), typeid(int));

            class Test {};
            Test *t = nullptr;
            CHECK_EQ(typeid(*t), typeid(Test));
        }
    }

    SUBCASE("dynamic_cast")
    {
        SUBCASE("primitive type")
        {
            class Base1 { public: virtual ~Base1() {} };
            class Base2 { public: virtual ~Base2() {} };
            class Derived1: public Base1, Base2 {};
            class Derived2: public Base1, Base2 {};

            Base1 *b1 = new Derived1;
            
            Derived1 *b1_1 = dynamic_cast<Derived1 *>(b1);
            CHECK_NE(b1_1, nullptr);

            Derived2 *b1_2 = dynamic_cast<Derived2 *>(b1);
            CHECK_EQ(b1_2, nullptr);

            Base2 *b2 = dynamic_cast<Base2 *>(b1);
        }
    }
}
