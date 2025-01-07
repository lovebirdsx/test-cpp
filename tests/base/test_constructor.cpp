#include <doctest.h>
#include <string>

TEST_CASE("constructor")
{
    SUBCASE("move constructor")
    {
        class FTest
        {
        public:
            bool MoveConstructorCalled = false;

            FTest() = default;

            FTest(FTest &&InTest)
            {
                InTest.MoveConstructorCalled = true;
            }
        };

        FTest Test;
        FTest Test2 = std::move(Test);

        CHECK(Test.MoveConstructorCalled);
    }
}
