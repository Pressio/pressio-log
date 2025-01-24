#include <gtest/gtest.h>

#include "helpers.hpp"
#include "pressio-log/core.hpp"

void runTest(pressiolog::LogLevel level) {
    CoutRedirector redirect;

    PRESSIOLOG_SET_LEVEL(level);
    PRESSIOLOG_SET_OUTPUT_STREAM(pressiolog::LogTo::console);

    PRESSIOLOG_BASIC("Basic");
    PRESSIOLOG_INFO("Info");
    PRESSIOLOG_DEBUG("Debug");
    PRESSIOLOG_WARNING("Warning");
    PRESSIOLOG_ERROR("Error");

    std::string output = redirect.str();

    EXPECT_TRUE(check_output(output, "Basic",   level >= pressiolog::LogLevel::basic));
    EXPECT_TRUE(check_output(output, "Info",    level >= pressiolog::LogLevel::info));
    EXPECT_TRUE(check_output(output, "Debug",   level >= pressiolog::LogLevel::debug));
    EXPECT_TRUE(check_output(output, "Warning", level >= pressiolog::LogLevel::basic));
    EXPECT_TRUE(check_output(output, "Error",   level >= pressiolog::LogLevel::none));
}

TEST(SerialLoggerTest, NoLogging) {
    runTest(pressiolog::LogLevel::none);
}

TEST(SerialLoggerTest, BasicLogLevel) {
    runTest(pressiolog::LogLevel::basic);
}

TEST(SerialLoggerTest, InfoLogLevel) {
    runTest(pressiolog::LogLevel::info);
}

TEST(SerialLoggerTest, DebugLogLevel) {
    runTest(pressiolog::LogLevel::debug);
}
