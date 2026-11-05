#include <cstdlib>

#include <gtest/gtest.h>

using std::system;

TEST(TrueIntegrationTest, ReturnsZero)
{
    const int result = system("./true");

    EXPECT_EQ(WEXITSTATUS(result), 0);
}