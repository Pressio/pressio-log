#include <gtest/gtest.h>
#include <filesystem>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

void runTest(pressiolog::LogTo dst) {
    PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::debug);
    PRESSIOLOG_SET_OUTPUT_STREAM(dst);

    CoutRedirector redirect;

    PRESSIOLOG_SPARSE("Sparse");
    PRESSIOLOG_INFO("Info");
    PRESSIOLOG_DEBUG("Debug");
    PRESSIOLOG_WARNING("Warning");
    PRESSIOLOG_ERROR("Error");

    if (dst == pressiolog::LogTo::console or
        dst == pressiolog::LogTo::both) {
        auto output = redirect.str();

        EXPECT_TRUE(check_output(output, "Sparse",  true));
        EXPECT_TRUE(check_output(output, "Info",    true));
        EXPECT_TRUE(check_output(output, "Debug",   true));
        EXPECT_TRUE(check_output(output, "Warning", true));
        EXPECT_TRUE(check_output(output, "Error",   true));
    }

    if (dst == pressiolog::LogTo::file or
        dst == pressiolog::LogTo::both) {
        const std::string fileName = "pressio.log";
        auto output = read_file(fileName);

        EXPECT_TRUE(check_output(output, "Sparse",  true));
        EXPECT_TRUE(check_output(output, "Info",    true));
        EXPECT_TRUE(check_output(output, "Debug",   true));
        EXPECT_TRUE(check_output(output, "Warning", true));
        EXPECT_TRUE(check_output(output, "Error",   true));

        std::filesystem::remove(fileName);
    }
}

TEST_F(LoggerTest, Serial_LogTo_Console) {
    runTest(pressiolog::LogTo::console);
}

TEST_F(LoggerTest, Serial_LogTo_File) {
    runTest(pressiolog::LogTo::file);
}

TEST_F(LoggerTest, Serial_LogTo_Both) {
    runTest(pressiolog::LogTo::both);
}
