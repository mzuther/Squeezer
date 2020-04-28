#include "unittest.h"


namespace
{

TEST(TestSuite2, FailingAddition)
{
    EXPECT_EQ(1 + 1, 3);
}

}  // namespace
