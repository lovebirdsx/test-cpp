#include <doctest.h>
#include <string>

TEST_CASE("enum")
{
    SUBCASE("unscoped")
    {
        enum ETest
        {
            Test1,
            Test2,
            Test3,
            Max,
        };

        ETest test = ETest::Test1;

        CHECK(test == ETest::Test1);

        int testInt = static_cast<int>(test);
        CHECK(testInt == 0);

        int max = static_cast<int>(ETest::Max);
        CHECK(max == 3);

        int max2 = static_cast<int>(Max);
        CHECK(max2 == 3);
    }

    SUBCASE("scoped")
    {
        enum class ETest
        {
            Test1,
            Test2,
            Test3,
            Max,
        };

        int max = static_cast<int>(ETest::Max);
        CHECK(max == 3);

        // int max2 = static_cast<int>(Max); // error
    }

    SUBCASE("enum class")
    {
        enum class ETest : unsigned int
        {
            Test1,
            Test2,
            Test3,
            Max,
        };

        unsigned int max = static_cast<unsigned int>(ETest::Max);
        CHECK(max == 3);

        // unsigned int max2 = static_cast<unsigned int>(Max); // error
    }

    SUBCASE("enum class with underlying type")
    {
        enum class EDirection : uint8_t
        {
            Up,
            Down,
            Left,
            Right,
            Max,
        };

        unsigned int max = static_cast<uint8_t>(EDirection::Max);
        CHECK(max == 4);

        // unsigned int max2 = static_cast<unsigned int>(Max); // error
    }

    SUBCASE("constexpr enum")
    {
        enum class EBufferSzie : size_t
        {
            Small = 256,
            Medium = 512,
            Large = 4024,
        };

        class Buffer
        {
        public:
            static constexpr size_t getBufferSize(EBufferSzie size)
            {
                return static_cast<size_t>(size);
            }
        };

        CHECK(Buffer::getBufferSize(EBufferSzie::Small) == 256);
    }

    SUBCASE("to string")
    {
        enum class HttpStatus : unsigned int
        {
            Ok = 200,
            BadRequest = 400,
            Unauthorized = 401,
            Forbidden = 403,
            NotFound = 404,
            InternalServerError = 500,
        };

        class Http
        {
        public:
            static std::string toString(HttpStatus status)
            {
                switch (status)
                {
                case HttpStatus::Ok:
                    return "Ok";
                case HttpStatus::BadRequest:
                    return "Bad Request";
                case HttpStatus::Unauthorized:
                    return "Unauthorized";
                case HttpStatus::Forbidden:
                    return "Forbidden";
                case HttpStatus::NotFound:
                    return "Not Found";
                case HttpStatus::InternalServerError:
                    return "Internal Server Error";
                default:
                    return "Unknown";
                }
            }
        };

        CHECK(Http::toString(HttpStatus::Ok) == "Ok");
        CHECK(Http::toString(HttpStatus::BadRequest) == "Bad Request");
    }
}
