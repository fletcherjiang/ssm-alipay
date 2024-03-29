/**
* @file math_utils.h
*
* Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <climits>
#include "acl/acl_base.h"
#include "common/log_inner.h"

namespace acl {

inline aclError CheckSizeTMultiOverflow(size_t a, size_t b, size_t &res)
{
    if ((a != 0U) && (b != 0U) && ((static_cast<size_t>(ULONG_MAX) / a) < b)) {
        ACL_LOG_ERROR("[Check][Overflow]%zu multiplies %zu overflow", a, b);
        return ACL_ERROR_FAILURE;
    }
    res = a * b;
    return ACL_SUCCESS;
}

inline aclError CheckUint32MultiOverflow(uint32_t a, uint32_t b, uint32_t &res)
{
    if ((a != 0U) && (b != 0U) && ((UINT32_MAX / a) < b)) {
        ACL_LOG_ERROR("[Check][Overflow]%u multiplies %u overflow", a, b);
        return ACL_ERROR_FAILURE;
    }
    res = a * b;
    return ACL_SUCCESS;
}

inline aclError CheckIntAddOverflow(int32_t a, int32_t b, int32_t &res)
{
    if (((b > 0) && (a > (INT_MAX - b))) || ((b < 0) && (a < (INT_MIN - b)))) {
        ACL_LOG_ERROR("[Check][Overflow]%d adds %d overflow", a, b);
        return ACL_ERROR_FAILURE;
    }
    res = a + b;
    return ACL_SUCCESS;
}

inline aclError CheckSizeTAddOverflow(size_t a, size_t b, size_t &res)
{
    if (a > (SIZE_MAX - b)) {
        ACL_LOG_ERROR("[Check][Overflow]%zu adds %zu overflow", a, b);
        return ACL_ERROR_FAILURE;
    }
    res = a + b;
    return ACL_SUCCESS;
}

inline aclError CheckUint32AddOverflow(uint32_t a, uint32_t b, uint32_t &res)
{
    if (a > (UINT32_MAX - b)) {
        ACL_LOG_ERROR("[Check][Overflow]%u adds %u overflow", a, b);
        return ACL_ERROR_FAILURE;
    }
    res = a + b;
    return ACL_SUCCESS;
}

aclError GetAlignedSize(size_t size, size_t &alignedSize);
} // namespace acl

#define ACL_CHECK_ASSIGN_SIZET_MULTI(a, b, res)                      \
    do {                                                             \
            const aclError ret = acl::CheckSizeTMultiOverflow(a, b, res);  \
            if (ret != ACL_SUCCESS) {                             \
                return ret;                                          \
            }                                                        \
    } while (false)

#define ACL_CHECK_ASSIGN_SIZET_MULTI_RET_NUM(a, b, res)                      \
    do {                                                             \
            const aclError ret = acl::CheckSizeTMultiOverflow(a, b, res);  \
            if (ret != ACL_SUCCESS) {                             \
                return 0;                                          \
            }                                                       \
    } while (false)

#define ACL_CHECK_ASSIGN_UINT32_MULTI(a, b, res)                      \
    do {                                                             \
            const aclError ret = acl::CheckUint32MultiOverflow(a, b, res);  \
            if (ret != ACL_SUCCESS) {                             \
                return ret;                                          \
            }                                                        \
    } while (false)

#define ACL_CHECK_ASSIGN_INT32_ADD(a, b, res)                        \
    do {                                                             \
            const aclError ret = acl::CheckIntAddOverflow(a, b, res);      \
            if (ret != ACL_SUCCESS) {                             \
                return ret;                                          \
            }                                                        \
    } while (false)

#define ACL_CHECK_ASSIGN_SIZET_ADD(a, b, res)                       \
    do {                                                            \
            const aclError ret = acl::CheckSizeTAddOverflow(a, b, res);   \
            if (ret != ACL_SUCCESS) {                            \
                return ret;                                         \
            }                                                       \
    } while (false)

#define ACL_CHECK_ASSIGN_UINT32T_ADD(a, b, res)                       \
    do {                                                            \
            const aclError ret = acl::CheckUint32AddOverflow(a, b, res);   \
            if (ret != ACL_SUCCESS) {                            \
                return ret;                                         \
            }                                                       \
    } while (false)

#endif // MATH_UTILS_H
