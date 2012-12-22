#include "sky/expected.hpp"
#include "gtest/gtest.h"

TEST(Expected, ConstructWithType)
{
    expected<int> x(0);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int)x);
    EXPECT_EQ(0, (int)(const expected<int>&)x);
}

TEST(Expected, ConstructWithPointer)
{
    expected<int*> x(nullptr);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(nullptr, (int*)x);
    EXPECT_EQ(nullptr, (int*)(const expected<int*>&)x);
}

TEST(Expected, ConstructWithReference)
{
    int i = 0;
    expected<int&> x(i);
    EXPECT_TRUE(x.valid());
    EXPECT_EQ(0, (int&)x);
    EXPECT_EQ(0, (int)(const expected<int&>&)x);
}

TEST(Expected, ConstructWithError)
{
    expected<int> x(error(5));
    EXPECT_FALSE(x.valid());

    try {
        (void)(int)x;
        FAIL();
    } catch (int err) {
        EXPECT_EQ(5, err);
    }

    try {
        (void)(int)(const expected<int>&)x;
        FAIL();
    } catch (int err) {
        EXPECT_EQ(5, err);
    }
}

TEST(Expected, ConstructWithCurrentException)
{
    try {
        throw 5;

    } catch (...) {

        expected<int> x = error(); // Fix after uniform initialization works
        EXPECT_FALSE(x.valid());

        try {
            (void)(int)x;
            FAIL();
        } catch (int err) {
            EXPECT_EQ(5, err);
        }

        try {
            (void)(int)(const expected<int>&)x;
            FAIL();
        } catch (int err) {
            EXPECT_EQ(5, err);
        }
    }
}
