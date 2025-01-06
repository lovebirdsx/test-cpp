#include <doctest.h>
#include <string>

TEST_CASE("assign")
{
    SUBCASE("assign operator")
    {
        class FTest
        {
        public:
            bool AssignCalled = false;

            FTest &operator=(const FTest &InOther)
            {
                AssignCalled = true;
                return *this;
            }
        };

        FTest Test1;
        FTest Test2;
        Test1 = Test2;
        CHECK(Test1.AssignCalled);
    }
}
