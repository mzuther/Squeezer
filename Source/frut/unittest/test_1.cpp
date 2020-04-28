#include "unittest.h"


namespace
{

TEST(TestSuite1, Addition)
{
    EXPECT_GT(1 + 1, 1);
    EXPECT_EQ(1 + 1, 2);
}

}  // namespace
