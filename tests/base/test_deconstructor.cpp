#include <doctest.h>
#include <string>

TEST_CASE("deconstructor")
{
    SUBCASE("virtual destructor")
    {
        static int baseConstructCallCount;
        static int baseDestructCallCount;
        static int derivedConstructCallCount;
        static int derivedDestructCallCount;

        baseConstructCallCount = 0;
        baseDestructCallCount = 0;
        derivedConstructCallCount = 0;
        derivedDestructCallCount = 0;

        class FBase
        {
        public:
            FBase()
            {
                baseConstructCallCount++;
            }

            virtual ~FBase()
            {
                baseDestructCallCount++;
            }
        };

        class FDerived : public FBase
        {
        public:
            FDerived()
            {
                derivedConstructCallCount++;
            }

            virtual ~FDerived() override
            {
                derivedDestructCallCount++;
            }
        };

        const FBase *base = new FDerived();
        delete base;

        CHECK(baseConstructCallCount == 1);
        CHECK(baseDestructCallCount == 1);
        CHECK(derivedConstructCallCount == 1);
        CHECK(derivedDestructCallCount == 1);
    }
}
