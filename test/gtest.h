#include <type_traits>

#include "gtest/gtest.h"

#define SKYTEST_STD_BOOLEAN_TRAIT2(trait, english, T, U) \
    (std::trait<T,U>::value)? ::testing::AssertionSuccess() : \
        ::testing::AssertionFailure() << "Expected " #T " to be " \
                                      << english << " " #U ". "

#define EXPECT_SAME(T, U) \
    GTEST_ASSERT_(SKYTEST_STD_BOOLEAN_TRAIT2(is_same, "same as", T, U), \
                  GTEST_NONFATAL_FAILURE_)
