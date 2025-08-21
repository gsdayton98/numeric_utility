#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "isqrt.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::exception;

int retcode = EXIT_SUCCESS;
int tests = 0;
int failures = 0;

void
test(int &tcode, int &ntests, int &nfailures, const char *str, bool b) {
    ++ntests;
    if (!b) {
        if (tcode == EXIT_SUCCESS)
            tcode = EXIT_FAILURE;
        ++nfailures;

        cerr << "Test failed " << str << endl;
    }
}

#define TEST(x) test(retcode, tests, failures, #x, x)

int
main(int argc, char *argv[]) {
    try {
        for (int trial = 0; trial <= 10000; ++trial) {
            int y = sqrt(trial);
            TEST(y * y <= trial && (y + 1) * (y + 1) > trial);
        }

        cout << tests << " run with " << failures << " failures" << endl;
    }
    catch (const exception &ex) {
        cerr << "Exception: " << ex.what() << endl;
    }

    return retcode;
}

