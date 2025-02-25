#include <gtest/gtest.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

void runTest() {
    PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::sparse);

    CoutRedirector redirect;

    int a = 1;
    PRESSIOLOG_SPARSE("Test, {:>3}", a);
    PRESSIOLOG_SPARSE("Left aligned: {:<10}", 42);
    PRESSIOLOG_SPARSE("Right aligned: {:>10}", 42);
    PRESSIOLOG_SPARSE("Center aligned: {:^10}", 42);

    std::string output = redirect.str();

    EXPECT_TRUE(check_output(output, "Test,   1", true))
        << "\nOutput:\n" << output;

    EXPECT_TRUE(check_output(output, "Left aligned: 42        ",  true))
        << "\nOutput:\n" << output;

    EXPECT_TRUE(check_output(output, "Right aligned:         42", true))
        << "\nOutput:\n" << output;

    EXPECT_TRUE(check_output(output, "Center aligned:     42    ", true))
        << "\nOutput:\n" << output;
}

TEST_F(LoggerTest, Serial_Formatting) {
    runTest();
}
