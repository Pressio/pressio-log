#include <gtest/gtest.h>

#include <mpi.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

using pressiolog::LogLevel;

bool condition(LogLevel logging_level, LogLevel tested_level, int target_rank, int current_rank) {
    return (logging_level >= tested_level) and (target_rank == current_rank);
}

void runTest(LogLevel level) {
    PRESSIOLOG_SET_LEVEL(level);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        throw std::runtime_error("Test requires comm size >= 3");
    }

    CoutRedirector redirect;

    PRESSIOLOG_BASIC("Basic");
    PRESSIOLOG_INFO("Info");

    PRESSIOLOG_SET_LOGGING_RANK(1);
    PRESSIOLOG_DEBUG("Debug");

    PRESSIOLOG_SET_LOGGING_RANK(2);
    PRESSIOLOG_WARNING("foo");   // will be prefixed by "WARNING: "
    PRESSIOLOG_ERROR("bar");     // will be prefixed by "ERROR: "

    std::string output = redirect.str();

    EXPECT_TRUE(check_output(output, "[0] Basic",   condition(level, LogLevel::basic, 0, rank)));
    EXPECT_TRUE(check_output(output, "[0] Info",    condition(level, LogLevel::info,  0, rank)));
    EXPECT_TRUE(check_output(output, "[1] Debug",   condition(level, LogLevel::debug, 1, rank)));
    EXPECT_TRUE(check_output(output, "[2] WARNING", condition(level, LogLevel::info,  2, rank)));
    EXPECT_TRUE(check_output(output, "[2] ERROR",   condition(level, LogLevel::info,  2, rank)));

    MPI_Barrier(MPI_COMM_WORLD);
}

TEST_F(LoggerTest, Parallel_LogLevel_None) {
    runTest(LogLevel::none);
}

TEST_F(LoggerTest, Parallel_LogLevel_Basic) {
    runTest(LogLevel::basic);
}

TEST_F(LoggerTest, Parallel_LogLevel_Info) {
    runTest(LogLevel::info);
}

TEST_F(LoggerTest, Parallel_LogLevel_Debug) {
    runTest(LogLevel::debug);
}
