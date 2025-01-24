#include <gtest/gtest.h>
#include <filesystem>

#include "helpers.hpp"
#include "pressio-log/core.hpp"

void runTest(pressiolog::LogTo dst) {
    CoutRedirector redirect;

    PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::debug);
    PRESSIOLOG_SET_OUTPUT_STREAM(dst);

    PRESSIOLOG_BASIC("Basic");
    PRESSIOLOG_INFO("Info");
    PRESSIOLOG_DEBUG("Debug");
    PRESSIOLOG_WARNING("Warning");
    PRESSIOLOG_ERROR("Error");

    if (dst == pressiolog::LogTo::console or
        dst == pressiolog::LogTo::both) {
        auto output = redirect.str();

        EXPECT_TRUE(check_output(output, "Basic",   true));
        EXPECT_TRUE(check_output(output, "Info",    true));
        EXPECT_TRUE(check_output(output, "Debug",   true));
        EXPECT_TRUE(check_output(output, "Warning", true));
        EXPECT_TRUE(check_output(output, "Error",   true));
    }

    if (dst == pressiolog::LogTo::file or
        dst == pressiolog::LogTo::both) {
        const std::string fileName = "pressio.log";
        auto output = read_file(fileName);

        EXPECT_TRUE(check_output(output, "Basic",   true));
        EXPECT_TRUE(check_output(output, "Info",    true));
        EXPECT_TRUE(check_output(output, "Debug",   true));
        EXPECT_TRUE(check_output(output, "Warning", true));
        EXPECT_TRUE(check_output(output, "Error",   true));

        std::filesystem::remove(fileName);
    }
}

TEST(SerialLoggerTest, LogToConsole) {
    runTest(pressiolog::LogTo::console);
}

TEST(SerialLoggerTest, LogToFile) {
    runTest(pressiolog::LogTo::file);
}

TEST(SerialLoggerTest, LogToBoth) {
    runTest(pressiolog::LogTo::both);
}
