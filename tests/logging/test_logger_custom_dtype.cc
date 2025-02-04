#include <gtest/gtest.h>

#include "helpers.hpp"
#include "LoggerTest.hpp"
#include "pressio-log/core.hpp"

template <typename T>
struct DataType { T val; };

template <typename T>
void runTest(T value) {
    CoutRedirector redirect;
    DataType<T> d{value};
    PRESSIOLOG_BASIC("Custom datatype: {}", d.val);
    std::string output = redirect.str();
    EXPECT_TRUE(check_output(output, "Custom datatype: " + std::to_string(value), true));
}

TEST_F(LoggerTest, Serial_CustomDataType_int) {
    runTest<int>(3);
}

TEST_F(LoggerTest, Serial_CustomDataType_double) {
    runTest<double>(3.3);
}
