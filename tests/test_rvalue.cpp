#include <doctest.h>
#include <string>

TEST_CASE("rvalue")
{
    SUBCASE("rvalue parameter")
    {
        class FTest
        {
        public:
            bool Print1Called = false;
            bool Print2Called = false;
            bool Print3Called = false;

            void Reset()
            {
                Print1Called = false;
                Print2Called = false;
                Print3Called = false;
            }

            void Print(const std::string &InString)
            {
                Print1Called = true;
            }

            void Print(std::string &&InString)
            {
                Print2Called = true;
            }

            void Print(std::string &InString)
            {
                Print3Called = true;
            }
        };

        FTest Test;

        std::string TestString = "TestString";
        Test.Print(TestString);
        CHECK(Test.Print3Called);
        Test.Reset();

        const std::string TestString2 = "TestString2";
        Test.Print(TestString2);
        CHECK(Test.Print1Called);
        Test.Reset();

        Test.Print(std::move(TestString));
        CHECK(Test.Print2Called);
        Test.Reset();

        Test.Print("TestString");
        CHECK(Test.Print2Called);
        Test.Reset();
    }
}
