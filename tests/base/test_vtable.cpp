#include <doctest.h>
#include "util.h"

#define POINT_SIZE sizeof(void*)

TEST_CASE("vtable")
{
    SUBCASE("virtual function")
    {
        class Base
        {
        public:
            virtual void f() {}
            virtual void g() {}
        };

        class Derived : public Base
        {
        public:
            void f() override {}
            void g() override {}
        };

        // 单继承，只有一个虚表，虚表指针占用8字节
        CHECK_EQ(sizeof(Base), POINT_SIZE);
        CHECK_EQ(sizeof(Derived), POINT_SIZE);
    }

    SUBCASE("multiple inheritance")
    {
        class A
        {
        public:
            virtual int f() { return  1; }
        };

        class B
        {
        public:
            virtual int g() { return  2; }
        };

        class C : public A, public B
        {
        public:
            virtual int h() { return  3; }
            int i() { return  4; }
        };

        // 多继承，C中有两个虚表，A和B的虚表指针各占用8字节，C为16字节
        CHECK_EQ(sizeof(A), POINT_SIZE);
        CHECK_EQ(sizeof(B), POINT_SIZE);
        CHECK_EQ(sizeof(C), POINT_SIZE * 2);

        C c;
        A *a = &c;
        B *b = &c;
        
        CHECK_EQ((long)a, (long)&c);
        // a和b指向不同的虚表，b的虚表指针在a的后面
        CHECK_EQ((long)b, (long)a + POINT_SIZE);

        CHECK_EQ(a->f(), 1);
        CHECK_EQ(b->g(), 2);
        CHECK_EQ(c.h(), 3);
        CHECK_EQ(c.i(), 4);
    }
}
