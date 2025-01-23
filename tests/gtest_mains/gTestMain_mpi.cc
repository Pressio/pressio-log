#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc,argv);
  int err = 0;
  {
    int rank = {};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    err = RUN_ALL_TESTS();

    MPI_Finalize();
  }
  return err;
}
