#include <gtest/gtest.h>

#include <mpi.h>

#include "helpers.hpp"
#include "pressio-log/core.hpp"

using pressiolog::LogLevel;

void runTest(pressiolog::LogTo dst) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        throw std::runtime_error("Test requires comm size >= 3");
    }

    CoutRedirector redirect;

    PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::debug);
    PRESSIOLOG_SET_OUTPUT_STREAM(dst);

    PRESSIOLOG_BASIC("Basic");
    PRESSIOLOG_INFO("Info", 0);
    PRESSIOLOG_DEBUG("Debug", 1);
    PRESSIOLOG_WARNING("foo", 2);   // will be prefixed by "WARNING: "
    PRESSIOLOG_ERROR("bar");        // will be prefixed by "ERROR: "

    MPI_Barrier(MPI_COMM_WORLD);

    if (dst == pressiolog::LogTo::console or
        dst == pressiolog::LogTo::both) {
        std::string output = redirect.str();

        EXPECT_TRUE(check_output(output, "[0] Basic",   0 == rank));
        EXPECT_TRUE(check_output(output, "[0] Info",    0 == rank));
        EXPECT_TRUE(check_output(output, "[1] Debug",   1 == rank));
        EXPECT_TRUE(check_output(output, "[2] WARNING", 2 == rank));
        EXPECT_TRUE(check_output(output, "[0] ERROR",   0 == rank));
    }

    if (rank == 0 and (
        dst == pressiolog::LogTo::file or
        dst == pressiolog::LogTo::both)) {
        const std::string fileName = "pressio.log";
        auto output = read_file(fileName);

        EXPECT_TRUE(check_output(output, "[0] Basic",   true));
        EXPECT_TRUE(check_output(output, "[0] Info",    true));
        EXPECT_TRUE(check_output(output, "[1] Debug",   true));
        EXPECT_TRUE(check_output(output, "[2] WARNING", true));
        EXPECT_TRUE(check_output(output, "[0] ERROR",   true));

        std::filesystem::remove(fileName);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(ParallelLoggerTest, LogToConsole) {
    runTest(pressiolog::LogTo::console);
}

TEST(ParallelLoggerTest, LogToFile) {
    runTest(pressiolog::LogTo::file);
}

TEST(ParallelLoggerTest, LogToBoth) {
    runTest(pressiolog::LogTo::both);
}
