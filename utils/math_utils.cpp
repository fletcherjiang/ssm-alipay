/**
* @file math_utils.cpp
*
* Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "math_utils.h"
#include "acl/acl_base.h"
#include "common/log_inner.h"

namespace acl {
aclError GetAlignedSize(size_t size, size_t &alignedSize)
{
    const int32_t DATA_MEMORY_ALIGN_SIZE = 32;
    // check overflow, the max value of size must be less than 0xFFFFFFFFFFFFFFFF-32*2
    if (size + static_cast<size_t>(DATA_MEMORY_ALIGN_SIZE * 2) < size) {
        ACL_LOG_INNER_ERROR("[Check][Size]size too large: %zu", size);
        return ACL_ERROR_INVALID_PARAM;
    }

    // align size to multiple of 32 and puls 32
    alignedSize = (size + static_cast<size_t>(DATA_MEMORY_ALIGN_SIZE * 2) - 1U) / DATA_MEMORY_ALIGN_SIZE * DATA_MEMORY_ALIGN_SIZE;
    return ACL_SUCCESS;
}
} // namespace acl

