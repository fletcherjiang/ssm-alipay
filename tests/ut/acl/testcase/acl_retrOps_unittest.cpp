#include "retr/retr_internal.h"

#include <gtest/gtest.h>

#include "acl/acl.h"
#include "securec.h"

using namespace std;
using namespace testing;

class UTEST_ACL_RetrOps : public testing::Test {
protected:

    void SetUp() {}

    void TearDown() {}
};

TEST_F(UTEST_ACL_RetrOps, aclCheckResultSuccessTest)
{
    rtStream_t stream;
    int32_t result = 0;
    void *retCode = (void *)&result;
    auto ret = acl::retr::aclCheckResult(retCode);
    EXPECT_EQ(ret, ACL_SUCCESS);
}