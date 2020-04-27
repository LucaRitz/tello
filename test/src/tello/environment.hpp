#pragma once
#define _WINSOCKAPI_

#include <gtest/gtest.h>

namespace tello {
    class Environment : public ::testing::Environment {
    public:
        ~Environment() override;

        // Override this to define how to set up the environment.
        void SetUp() override;

        // Override this to define how to tear down the environment.
        void TearDown() override;
    };
}