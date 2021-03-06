#define CATCH_CONFIG_MAIN
#include "../libs/catch.h"
#include "../Templates.h"

unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Should create Array")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Calling _void_f")
{
    Templates::Meta::_void_f();
}
