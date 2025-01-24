#include <gtest/gtest.h>

#include <mpi.h>

#include "helpers.hpp"
#include "pressio-log/core.hpp"

void runTest() {
    int orig_rank, orig_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &orig_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &orig_size);

    if (orig_size < 3) {
        throw std::runtime_error("Test requires comm size >= 3");
    }

    int color = orig_rank % 2;
    int key = orig_rank;
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &new_comm);

    int rank, size;
    MPI_Comm_rank(new_comm, &rank);
    MPI_Comm_size(new_comm, &size);

    EXPECT_LT(size, orig_size);

    PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::debug);
    PRESSIOLOG_SET_OUTPUT_STREAM(pressiolog::LogTo::console);

    // Check that new comm is being used (nothing should print on too-high rank)
    CoutRedirector empty_redirect;
    PRESSIOLOG_DEBUG("Should be empty", orig_size - 1, new_comm);
    std::string empty_output = empty_redirect.str();
    EXPECT_TRUE(check_output(empty_output, "Should be empty", false));

    // Check that new comm works properly
    CoutRedirector redirect;
    PRESSIOLOG_INFO("Info", 1)
    PRESSIOLOG_DEBUG("Debug");
    std::string output = redirect.str();
    EXPECT_TRUE(check_output(output, "[1] Info",  rank == 1));
    EXPECT_TRUE(check_output(output, "[0] Debug", rank == 0));
}

TEST(ParallelLoggerTest, ChangeCommunicator) {
    runTest();
}