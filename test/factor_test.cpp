
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <vector>
#include "factor.hpp"
using namespace utility;

BOOST_AUTO_TEST_CASE(testFactorEvaluate) {
    const std::vector testCase12 {Factor {2,2}, Factor {3,1}};

    BOOST_CHECK_EQUAL(Factor::evaluate(testCase12), 12U);

    const std::vector testCase220 {Factor {2,2}, Factor{5,1}, Factor{11,1}};
    BOOST_CHECK_EQUAL(Factor::evaluate(testCase220), 220U);
}


BOOST_AUTO_TEST_CASE(testFactor) {
    const auto factors12 = Factor::factor(12U);
    BOOST_CHECK_EQUAL(factors12.size(), 2U);
    BOOST_TEST( (factors12[0] == Factor{2, 2}) );
    BOOST_TEST( (factors12[1] == Factor{3, 1}) );


    const auto factors220 = Factor::factor(220U);
    BOOST_CHECK_EQUAL(factors220.size(), 3U);
    BOOST_TEST( (factors220[0] == Factor{2, 2}) );
    BOOST_TEST( (factors220[1] == Factor{5, 1}) );
    BOOST_TEST( (factors220[2] == Factor{11, 1}) );
}


BOOST_AUTO_TEST_CASE(testOperators) {
    Factor leftOperand {2, 3};
    Factor rightOperand {5, 2};
    constexpr Factor anotherOperand {5, 3};
    constexpr Factor yetAnotherOperand {2, 3};

    BOOST_CHECK( (leftOperand == yetAnotherOperand) );
    BOOST_CHECK( (leftOperand != rightOperand) );
    BOOST_CHECK( (rightOperand != anotherOperand) );

    BOOST_CHECK( leftOperand < rightOperand );
    BOOST_CHECK( (rightOperand < anotherOperand) );
}


BOOST_AUTO_TEST_CASE(testInserter) {
    std::ostringstream output;
    constexpr Factor example {5, 2};
    output << example;
    BOOST_CHECK_EQUAL(output.str(), "5^2");
}

BOOST_AUTO_TEST_CASE(testPrimeList)
{
    for (auto n=4u; n < 0xFFFFu; ++n) {
        auto factors = Factor::factor(n);
        if (auto result = Factor::evaluate(factors); n != result ) {
            BOOST_REQUIRE_EQUAL(n, result);
        }
    }
    BOOST_CHECK(true);
}
