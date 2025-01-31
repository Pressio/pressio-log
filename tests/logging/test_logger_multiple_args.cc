#include <gtest/gtest.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

void runTest() {
    CoutRedirector redirect;
    PRESSIOLOG_BASIC("arg1", "arg2", 5);
    std::string output = redirect.str();
    EXPECT_TRUE(check_output(output, "arg1 arg2 5", true));
}

TEST_F(LoggerTest, Serial_MultipleArgs) {
    runTest();
}
