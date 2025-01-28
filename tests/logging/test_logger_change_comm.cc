#include <gtest/gtest.h>

#include <mpi.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

bool condition(int exp_rank, int act_rank, int color) {
    return (exp_rank == act_rank) and (color == 0);
}

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
    PRESSIOLOG_SET_COMMUNICATOR(new_comm);

    {
        // Check that new comm is being used (warning should print on too-high rank)
        CoutRedirector invalid_redirect;
        PRESSIOLOG_DEBUG("Invalid rank", orig_size - 1);
        std::string invalid_output = invalid_redirect.str();
        EXPECT_TRUE(check_output(invalid_output, "WARNING", true));
    }

    {
        // Check that new comm works properly
        CoutRedirector redirect;
        PRESSIOLOG_INFO("Info", 0)
        PRESSIOLOG_DEBUG("Debug");
        std::string output = redirect.str();
        EXPECT_TRUE(check_output(output, "[0] Info",  condition(0, rank, color)));
        EXPECT_TRUE(check_output(output, "[0] Debug", condition(0, rank, color)));
    }
}

TEST_F(LoggerTest, Parallel_ChangeCommunicator) {
    runTest();
}
