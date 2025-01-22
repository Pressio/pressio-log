// #include <gtest/gtest.h>

#include "pressio-log/core.hpp"

int main() {
    int a = 1;
    PRESSIOLOG_BASIC("Basic log");
    PRESSIOLOG_INFO("Info log");
    PRESSIOLOG_DEBUG("Debug log");
    PRESSIOLOG_WARNING("Warning log");
    PRESSIOLOG_ERROR("Error log");
}
