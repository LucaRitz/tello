#include "gtest/gtest.h"
#include "environment.hpp"

#define NDEBUG

using tello::Environment;

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::Environment* const foo_env =
            ::testing::AddGlobalTestEnvironment(new Environment());

    return RUN_ALL_TESTS();
}