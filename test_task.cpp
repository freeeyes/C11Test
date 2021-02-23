#include <iostream>
#include <gtest/gtest.h>

TEST(run_test_iotoio, test_iotoio)
{
	int a = 1 + 2;
    EXPECT_EQ(a, 3);
}

int main(int argc, char* argv[])
{
    //‘À––google test ≤‚ ‘”√¿˝
    ::testing::GTEST_FLAG(output) = "xml:Test_add.xml";
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}