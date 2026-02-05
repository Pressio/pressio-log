#include <gtest/gtest.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

TEST_F(LoggerTest, Logger_String_Formatting) {
    PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::debug);

    CoutRedirector redirect;

    PRESSIOLOG_SPARSE("{}", "hey there");
    PRESSIOLOG_SPARSE("{} {}", "two", "strings");
    PRESSIOLOG_SPARSE("{}", "Debug");
    PRESSIOLOG_SPARSE("{}: {}", 2.1, "number");
    PRESSIOLOG_SPARSE("{}", "Error");

    std::string output = redirect.str();

    EXPECT_TRUE(check_output(output, "hey there",   true));
    EXPECT_TRUE(check_output(output, "two strings", true));
    EXPECT_TRUE(check_output(output, "Debug",       true));
    EXPECT_TRUE(check_output(output, "2.1: number",   true));
    EXPECT_TRUE(check_output(output, "Error",       true));
    EXPECT_TRUE(check_output(output, "could not format given string",   false));
}
