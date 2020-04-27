#include "FrutHeader.h"
#include <cxxtest/TestSuite.h>


class MyTestSuite2 : public CxxTest::TestSuite
{
public:
    void testAddition(void)
    {
        TS_ASSERT_EQUALS(1 + 1, 3);
    }
};
