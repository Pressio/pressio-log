#include <gtest/gtest.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

TEST_F(LoggerTest, Logger_String_Templated) {
    PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::debug);

    CoutRedirector redirect;

    PRESSIOLOG_SPARSE("{}", "hey there");
    PRESSIOLOG_INFO("{} {}", "two", "strings");
    PRESSIOLOG_DEBUG("{}", "Debug");
    PRESSIOLOG_WARNING("{}: {}", 2, "number");
    PRESSIOLOG_ERROR("{}", "Error");

    std::string output = redirect.str();

    EXPECT_TRUE(check_output(output, "hey there",   true));
    EXPECT_TRUE(check_output(output, "two strings", true));
    EXPECT_TRUE(check_output(output, "Debug",       true));
    EXPECT_TRUE(check_output(output, "2: number",   true));
    EXPECT_TRUE(check_output(output, "Error",       true));
}
