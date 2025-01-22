#include <gtest/gtest.h>

#include "helpers.hpp"
#include "pressio-log/core.hpp"

void runTest(pressiolog::level level) {
    CoutRedirector redirect;

    PRESSIOLOG_SET_LEVEL(level);

    PRESSIOLOG_BASIC("Basic");
    PRESSIOLOG_INFO("Info");
    PRESSIOLOG_DEBUG("Debug");
    PRESSIOLOG_WARNING("Warning");
    PRESSIOLOG_ERROR("Error");

    std::string output = redirect.str();
    EXPECT_TRUE(find_in_output(output, "Basic", level >= 1));
    EXPECT_TRUE(find_in_output(output, "Info", level >= 2));
    EXPECT_TRUE(find_in_output(output, "Debug", level >= 3));
    EXPECT_TRUE(find_in_output(output, "Warning", level >= 1));
    EXPECT_TRUE(find_in_output(output, "Error", level >= 0));
}

TEST(LoggerTest, NoLogging) {
    runTest(pressiolog::level::none);
}

TEST(LoggerTest, BasicLogLevel) {
    runTest(pressiolog::level::basic);
}

TEST(LoggerTest, InfoLogLevel) {
    runTest(pressiolog::level::info);
}

TEST(LoggerTest, DebugLogLevel) {
    runTest(pressiolog::level::debug);
}
