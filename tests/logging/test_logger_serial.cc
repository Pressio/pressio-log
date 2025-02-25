#include <gtest/gtest.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

void runTest(pressiolog::LogLevel level) {
    PRESSIOLOG_SET_LEVEL(level);

    CoutRedirector redirect;

    PRESSIOLOG_SPARSE("Sparse");
    PRESSIOLOG_INFO("Info");
    PRESSIOLOG_DEBUG("Debug");
    PRESSIOLOG_WARNING("Warning");
    PRESSIOLOG_ERROR("Error");

    std::string output = redirect.str();

    EXPECT_TRUE(check_output(output, "Sparse",  level >= pressiolog::LogLevel::sparse));
    EXPECT_TRUE(check_output(output, "Info",    level >= pressiolog::LogLevel::info));
    EXPECT_TRUE(check_output(output, "Debug",   level >= pressiolog::LogLevel::debug));
    EXPECT_TRUE(check_output(output, "Warning", level >= pressiolog::LogLevel::info));
    EXPECT_TRUE(check_output(output, "Error",   level >= pressiolog::LogLevel::info));
}

TEST_F(LoggerTest, Serial_LogLevel_None) {
    runTest(pressiolog::LogLevel::none);
}

TEST_F(LoggerTest, Serial_LogLevel_Sparse) {
    runTest(pressiolog::LogLevel::sparse);
}

TEST_F(LoggerTest, Serial_LogLevel_Info) {
    runTest(pressiolog::LogLevel::info);
}

TEST_F(LoggerTest, Serial_LogLevel_Debug) {
    runTest(pressiolog::LogLevel::debug);
}
