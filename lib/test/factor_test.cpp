
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <vector>
#include "factor.hpp"

BOOST_AUTO_TEST_CASE(testFactorEvaluate) {
    std::vector<Factor> testCase12 {Factor {2,2}, Factor {3,1}};

    BOOST_CHECK_EQUAL(Factor::evaluate(testCase12), 12U);

    std::vector<Factor> testCase220 {Factor {2,2}, Factor{5,1}, Factor{11,1}};
    BOOST_CHECK_EQUAL(Factor::evaluate(testCase220), 220U);
}


BOOST_AUTO_TEST_CASE(testFactor) {
    auto factors12 = Factor::factor(12U);
    BOOST_CHECK_EQUAL(factors12.size(), 2U);
    BOOST_TEST( (factors12[0] == Factor{2, 2}) );
    BOOST_TEST( (factors12[1] == Factor{3, 1}) );


    auto factors220 = Factor::factor(220U);
    BOOST_CHECK_EQUAL(factors220.size(), 3U);
    BOOST_TEST( (factors220[0] == Factor{2, 2}) );
    BOOST_TEST( (factors220[1] == Factor{5, 1}) );
    BOOST_TEST( (factors220[2] == Factor{11, 1}) );
}


BOOST_AUTO_TEST_CASE(testOperators) {
    Factor leftOperand {2, 3};
    Factor rightOperand {5, 2};
    Factor anotherOperand {5, 3};
    Factor yetAnotherOperand {2, 3};

    BOOST_TEST( (leftOperand == yetAnotherOperand) );
    BOOST_TEST( (leftOperand != rightOperand) );
    BOOST_TEST( (rightOperand != anotherOperand) );

    BOOST_TEST( leftOperand < rightOperand );
    BOOST_TEST( (rightOperand < anotherOperand) );
}


BOOST_AUTO_TEST_CASE(testInserter) {
    std::ostringstream output;
    Factor example {5, 2};
    output << example;
    BOOST_CHECK_EQUAL(output.str(), "5^2");
}

BOOST_AUTO_TEST_CASE(testPrimeList)
{
    for (auto n=4u; n < 0xFFFFu; ++n) {
        auto factors = Factor::factor(n);
        auto result = Factor::evaluate(factors);
        if (n != result ) {
            BOOST_REQUIRE_EQUAL(n, result);
        }
    }
    BOOST_CHECK(true);
}
