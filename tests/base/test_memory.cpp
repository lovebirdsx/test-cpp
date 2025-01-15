#include <doctest.h>

TEST_CASE("memory")
{
    SUBCASE("primitive")
    {
        CHECK_EQ(sizeof(int), 4);
        CHECK_EQ(sizeof(long), 8);
    }

    SUBCASE("class")
    {
        class A { int value; };
        CHECK_EQ(sizeof(A), 4);

        // 虚表占用8字节，value占用4字节，但是因为对齐的原因，所以整个类占用16字节
        class B { int value; virtual void f() {} };
        CHECK_EQ(sizeof(B), 16);

        // 相对于B，多了一个value2，刚好对齐到16字节
        class C { int value1; int value2; virtual void f() {} };
        CHECK_EQ(sizeof(C), 16);

        // 多个虚函数，虚表占用8字节，value1占用4字节，value2占用4字节
        class D { int value1; int value2; virtual void f() {}; virtual void g() {} };
        CHECK_EQ(sizeof(D), 16);
    }
}
