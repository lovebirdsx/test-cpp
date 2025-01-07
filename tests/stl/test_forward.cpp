#include <doctest.h>
#include <string>
#include <memory>
#include <utility>

namespace
{
    class Test
    {
    public:
        int processRefCallCount = 0;
        int processRValueCallCount = 0;

        template <typename T>
        void wrapper1(T &&x)
        {
            process(x);
        }

        template <typename T>
        void wrapper2(T &&x)
        {
            process(std::forward<T>(x));
        }

        template <typename T>
        void wrapper3(T &&x)
        {
            process(std::move(x));
        }

        void Reset()
        {
            processRefCallCount = 0;
            processRValueCallCount = 0;
        }

    private:
        void process(int &x)
        {
            processRefCallCount++;
        }

        void process(int &&x)
        {
            processRValueCallCount++;
        }
    };

    class Widget
    {
    public:
        Widget(int x, bool b=false): x(x), b(b) {}

        int getX() const { return x; }
        int getB() const { return b; }

        template <typename ...TArgs>
        static std::unique_ptr<Widget> makeWidget(TArgs &&...args)
        {
            return std::make_unique<Widget>(std::forward<TArgs>(args)...);
        }
    
    private:
        Widget(Widget const &) = delete;
        Widget &operator=(Widget const &) = delete;
        Widget(Widget &&) = delete;
        Widget &operator=(Widget &&) = delete;
        Widget() = delete;

        int x;
        bool b;
    };
}

TEST_CASE("forward")
{
    SUBCASE("forward in template")
    {
        Test test;
        int x = 0;
        test.wrapper1(x);
        test.wrapper1(0);
        CHECK(test.processRefCallCount == 2);

        test.Reset();
        test.wrapper2(x);
        test.wrapper2(0);

        CHECK(test.processRValueCallCount == 1);
        CHECK(test.processRefCallCount == 1);

        test.Reset();
        test.wrapper3(x);
        test.wrapper3(0);
        CHECK(test.processRValueCallCount == 2);
    }

    SUBCASE("factory")
    {
        auto widget = Widget::makeWidget(42);
        CHECK(widget->getX() == 42);
        CHECK(widget->getB() == false);

        auto widget2 = Widget::makeWidget(42, true);
        CHECK(widget2->getX() == 42);
        CHECK(widget2->getB() == true);
    }
}
