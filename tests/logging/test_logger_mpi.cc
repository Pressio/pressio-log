#include <gtest/gtest.h>

#include <mpi.h>

#include "helpers.hpp"
#include "pressio-log/core.hpp"

using pressiolog::LogLevel;

bool condition(LogLevel logging_level, LogLevel tested_level, int target_rank, int current_rank) {
    return (logging_level >= tested_level) and (target_rank == current_rank);
}

void runTest(LogLevel level) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        throw std::runtime_error("Test requires comm size >= 3");
    }

    CoutRedirector redirect;

    PRESSIOLOG_SET_LEVEL(level);

    PRESSIOLOG_BASIC("Basic");
    PRESSIOLOG_INFO("Info", 0);
    PRESSIOLOG_DEBUG("Debug", 1);
    PRESSIOLOG_WARNING("Warning", 2); // will be prefixed by "WARNING: "
    PRESSIOLOG_ERROR("Error");        // will be prefixed by "ERROR: "

    std::string output = redirect.str();

    EXPECT_TRUE(check_output(output, "[0] Basic",   condition(level, LogLevel::basic, 0, rank)));
    EXPECT_TRUE(check_output(output, "[0] Info",    condition(level, LogLevel::info,  0, rank)));
    EXPECT_TRUE(check_output(output, "[1] Debug",   condition(level, LogLevel::debug, 1, rank)));
    EXPECT_TRUE(check_output(output, "[2] WARNING", condition(level, LogLevel::basic, 2, rank)));
    EXPECT_TRUE(check_output(output, "[0] ERROR",   condition(level, LogLevel::none,  0, rank)));

    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(ParallelLoggerTest, NoLogging) {
    runTest(LogLevel::none);
}

TEST(ParallelLoggerTest, BasicLogLevel) {
    runTest(LogLevel::basic);
}

TEST(ParallelLoggerTest, InfoLogLevel) {
    runTest(LogLevel::info);
}

TEST(ParallelLoggerTest, DebugLogLevel) {
    runTest(LogLevel::debug);
}
