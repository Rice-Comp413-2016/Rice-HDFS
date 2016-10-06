#include "gtest/gtest.h"



TEST(IsEven, Four) {

EXPECT_TRUE(4 % 2 == 0);

}

TEST(IsEven, Three) {

EXPECT_FALSE(3 % 2 == 0);

}

/*
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/
