//
// Created by Glen Dayton on 8/29/25.
//
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "file.hpp"

BOOST_AUTO_TEST_CASE(test_file) {
    const char* fileName = "test_file.dat";
    const char* sample = "test";
    std::ofstream testFile(fileName);
    testFile << sample;
    testFile.write("", 1);
    testFile.close();


    oscpp::File fileUnderTest(fileName);
    auto mappedData = fileUnderTest.map();
    auto s = static_cast<const char *>(mappedData.first);
    BOOST_CHECK_EQUAL(strcmp(s, sample), 0);
    BOOST_CHECK_EQUAL(mappedData.second, strlen(sample) + 1);
}
