#include <gtest/gtest.h>
#include "pressio-log/core.hpp"

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        PRESSIOLOG_INITIALIZE();
    }

    void TearDown() override {
        PRESSIOLOG_FINALIZE();
    }
};
