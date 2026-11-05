#include <cstdlib>

#include <gtest/gtest.h>

using std::system;

TEST(TrueIntegrationTest, ReturnsOne)
{
    const int result = system("./false");

    EXPECT_EQ(WEXITSTATUS(result), 1);
}