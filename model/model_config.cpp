/**
* @file model_config.cpp
*
* Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
#include "model_config.h"
#include "executor/ge_executor.h"
#include "common/ge_inner_error_codes.h"
#include "framework/common/ge_types.h"
#include "runtime/dev.h"
#include "model_desc_internal.h"
#include "error_codes_inner.h"
#include "toolchain/resource_statistics.h"
#include "toolchain/profiling_manager.h"
#include "utils/math_utils.h"

namespace {
    typedef aclError (*CheckMdlConfigFunc)(const void *, size_t);
    typedef aclError (*SetMdlConfigFunc)(aclmdlConfigHandle *, const void *);

    struct SetMdlConfigParamFunc {
        CheckMdlConfigFunc checkFunc;
        SetMdlConfigFunc setFunc;
    };

    aclError CheckMdlLoadPriority(const void *attrValue, size_t valueSize)
    {
        ACL_LOG_INFO("start to execute CheckMdlLoadPriority.");
        if (valueSize != sizeof(int32_t)) {
            ACL_LOG_INNER_ERROR("[Check][File]valueSize[%zu] is invalid, it should be %zu", valueSize,
                sizeof(int32_t));
            return ACL_ERROR_INVALID_PARAM;
        }

        static bool mdlLoadPriorityFlag = false;
        static int32_t leastPriority;
        static int32_t greatestPriority;
        if (!mdlLoadPriorityFlag) {
            rtError_t rtErr = rtDeviceGetStreamPriorityRange(&leastPriority, &greatestPriority);
            if (rtErr != RT_ERROR_NONE) {
                ACL_LOG_CALL_ERROR("[Get][PriorityRange]get range of stream priority failed, "
                    "runtime result = %d", static_cast<int32_t>(rtErr));
                return ACL_GET_ERRCODE_RTS(rtErr);
            }
            mdlLoadPriorityFlag = true;
            ACL_LOG_INFO("execute rtDeviceGetStreamPriorityRange success, greatest load priority = %d, "
                "least load priority = %d", greatestPriority, leastPriority);
        }
        int32_t priority = *static_cast<const int32_t *>(attrValue);
        ACL_CHECK_RANGE_INT(priority, greatestPriority, leastPriority);
        ACL_LOG_INFO("successfully execute CheckMdlLoadPriority");
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadPriority(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadPriority.");
        int32_t priority = *static_cast<const int32_t *>(attrValue);
        handle->priority = priority;
        ACL_LOG_INFO("successfully execute SetMdlLoadPriority");
        return ACL_SUCCESS;
    }

    aclError CheckMdlLoadType(const void *attrValue, size_t valueSize)
    {
        ACL_LOG_INFO("start to execute CheckMdlLoadType.");
        if (valueSize != sizeof(size_t)) {
            ACL_LOG_INNER_ERROR("[Check][ValueSize]valueSize[%zu] is invalid, it should be %zu",
                valueSize, sizeof(size_t));
            return ACL_ERROR_INVALID_PARAM;
        }
        size_t type = *static_cast<const size_t *>(attrValue);
        if ((type < static_cast<size_t>(ACL_MDL_LOAD_FROM_FILE)) ||
            (type > static_cast<size_t>(ACL_MDL_LOAD_FROM_MEM_WITH_Q))) {
            ACL_LOG_INNER_ERROR("[Check][Type]type[%zu] is invalid, it should be in [%d, %d]",
                type, ACL_MDL_LOAD_FROM_FILE, ACL_MDL_LOAD_FROM_MEM_WITH_Q);
        }
        ACL_LOG_INFO("successfully execute CheckMdlLoadType to check aclmdlLoadType[%zu]", type);
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadType(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadType.");
        size_t type = *static_cast<const size_t *>(attrValue);
        handle->mdlLoadType = type;
        (void)handle->attrState.insert(ACL_MDL_LOAD_TYPE_SIZET);
        ACL_LOG_INFO("successfully execute SetMdlLoadType to set aclmdlLoadType[%zu]", type);
        return ACL_SUCCESS;
    }

    // compared with CheckMdlLoadPtrAttr, the CheckMdlLoadPtrAttrEx requires the values attrValue point to
    // can't be nullptr
    aclError CheckMdlLoadPtrAttrEx(const void *attrValue, size_t valueSize)
    {
        ACL_LOG_INFO("start to execute CheckMdlLoadPtrAttrEx.");
        if (valueSize != sizeof(void *)) {
            ACL_LOG_INNER_ERROR("[Check][ValueSize]valueSize[%zu] is invalid, it should be %zu",
                valueSize, sizeof(void *));
            return ACL_ERROR_INVALID_PARAM;
        }
        void *val = *static_cast<void * const *>(attrValue);
        ACL_REQUIRES_NOT_NULL(val);
        ACL_LOG_INFO("successfully execute CheckMdlLoadPtrAttrEx");
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadPath(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadPath");
        // set model path with deep copy
        char *loadPath = *static_cast<char * const *>(attrValue);
        handle->loadPath = loadPath;
        (void)handle->attrState.insert(ACL_MDL_PATH_PTR);
        ACL_LOG_INFO("successfully execute SetMdlLoadPath to set loadPath[%s]", loadPath);
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadMemPtr(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadMemPtr");
        void *val = *static_cast<void * const *>(attrValue);
        handle->mdlAddr = val;
        (void)handle->attrState.insert(ACL_MDL_MEM_ADDR_PTR);
        ACL_LOG_INFO("successfully execute SetMdlLoadMemPtr");
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadInputQPtr(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadInputQPtr");
        void *val = *static_cast<void * const *>(attrValue);
        handle->inputQ = static_cast<uint32_t *>(val);
        (void)handle->attrState.insert(ACL_MDL_INPUTQ_ADDR_PTR);
        ACL_LOG_INFO("successfully execute SetMdlLoadInputQPtr");
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadOutputQPtr(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadOutputQPtr");
        void *val = *static_cast<void * const *>(attrValue);
        handle->outputQ = static_cast<uint32_t *>(val);
        (void)handle->attrState.insert(ACL_MDL_OUTPUTQ_ADDR_PTR);
        ACL_LOG_INFO("successfully execute SetMdlLoadOutputQPtr");
        return ACL_SUCCESS;
    }

    aclError CheckMdlLoadSizeAttr(const void *attrValue, size_t valueSize)
    {
        ACL_LOG_INFO("start to execute CheckMdlLoadSizeAttr");
        if (valueSize != sizeof(size_t)) {
            ACL_LOG_INNER_ERROR("[Check][ValueSize]valueSize[%zu] is invalid, it should be %zu",
                valueSize, sizeof(size_t));
            return ACL_ERROR_INVALID_PARAM;
        }
        ACL_LOG_INFO("successfully execute CheckMdlLoadSizeAttr");
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadWeightSize(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadWeightSize");
        size_t weightSize = *static_cast<const size_t *>(attrValue);
        handle->weightSize = weightSize;
        ACL_LOG_INFO("successfully execute SetMdlLoadWeightSize to set weightSize[%zu]", weightSize);
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadWorkspaceSize(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadWorkspaceSize");
        size_t workSize = *static_cast<const size_t *>(attrValue);
        handle->workSize = workSize;
        ACL_LOG_INFO("successfully execute SetMdlLoadWorkspaceSize to set workSize[%zu]", workSize);
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadInputQNum(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadInputQNum");
        size_t num = *static_cast<const size_t *>(attrValue);
        handle->inputQNum = num;
        (void)handle->attrState.insert(ACL_MDL_INPUTQ_NUM_SIZET);
        ACL_LOG_INFO("successfully execute SetMdlLoadInputQNum to set inputQNum[%zu]", num);
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadOutputQNum(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadOutputQNum");
        size_t num = *static_cast<const size_t *>(attrValue);
        handle->outputQNum = num;
        (void)handle->attrState.insert(ACL_MDL_OUTPUTQ_NUM_SIZET);
        ACL_LOG_INFO("successfully execute SetMdlLoadOutputQNum to set outputQNum[%zu]", num);
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadMemSize(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadMemSize");
        size_t memSize = *static_cast<const size_t *>(attrValue);
        handle->mdlSize = memSize;
        (void)handle->attrState.insert(ACL_MDL_MEM_SIZET);
        ACL_LOG_INFO("successfully execute SetMdlLoadMemSize to set memSize[%zu]", memSize);
        return ACL_SUCCESS;
    }

    aclError CheckMdlLoadPtrAttr(const void *attrValue, size_t valueSize)
    {
        ACL_LOG_INFO("start to execute CheckMdlLoadPtrAttr");
        if (valueSize != sizeof(void *)) {
            ACL_LOG_INNER_ERROR("[Check][ValueSize]valueSize[%zu] is invalid, it should be %zu",
                valueSize, sizeof(void *));
            return ACL_ERROR_INVALID_PARAM;
        }
        ACL_LOG_INFO("successfully execute CheckMdlLoadPtrAttr");
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadWeightPtr(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadWeightPtr");
        void *val = *static_cast<void * const *>(attrValue);
        handle->weightPtr = val;
        ACL_LOG_INFO("successfully execute SetMdlLoadWeightPtr");
        return ACL_SUCCESS;
    }

    aclError SetMdlLoadWorkPtr(aclmdlConfigHandle *handle, const void *attrValue)
    {
        ACL_LOG_INFO("start to execute SetMdlLoadWorkPtr");
        void *val = *static_cast<void * const *>(attrValue);
        handle->workPtr = val;
        ACL_LOG_INFO("successfully execute SetMdlLoadWorkPtr");
        return ACL_SUCCESS;
    }

    std::map<aclmdlConfigAttr, SetMdlConfigParamFunc> g_setMdlConfigMap = {
        {ACL_MDL_PRIORITY_INT32, {&CheckMdlLoadPriority, &SetMdlLoadPriority}},
        {ACL_MDL_LOAD_TYPE_SIZET, {&CheckMdlLoadType, &SetMdlLoadType}},
        {ACL_MDL_PATH_PTR, {&CheckMdlLoadPtrAttrEx, &SetMdlLoadPath}},
        {ACL_MDL_MEM_ADDR_PTR, {&CheckMdlLoadPtrAttrEx, &SetMdlLoadMemPtr}},
        {ACL_MDL_INPUTQ_ADDR_PTR, {&CheckMdlLoadPtrAttrEx, &SetMdlLoadInputQPtr}},
        {ACL_MDL_OUTPUTQ_ADDR_PTR, {&CheckMdlLoadPtrAttrEx, &SetMdlLoadOutputQPtr}},
        {ACL_MDL_MEM_SIZET, {&CheckMdlLoadSizeAttr, &SetMdlLoadMemSize}},
        {ACL_MDL_WEIGHT_SIZET, {&CheckMdlLoadSizeAttr, &SetMdlLoadWeightSize}},
        {ACL_MDL_WORKSPACE_SIZET, {&CheckMdlLoadSizeAttr, &SetMdlLoadWorkspaceSize}},
        {ACL_MDL_INPUTQ_NUM_SIZET, {&CheckMdlLoadSizeAttr, &SetMdlLoadInputQNum}},
        {ACL_MDL_OUTPUTQ_NUM_SIZET, {&CheckMdlLoadSizeAttr, &SetMdlLoadOutputQNum}},
        {ACL_MDL_WEIGHT_ADDR_PTR, {&CheckMdlLoadPtrAttr, &SetMdlLoadWeightPtr}},
        {ACL_MDL_WORKSPACE_ADDR_PTR, {&CheckMdlLoadPtrAttr, &SetMdlLoadWorkPtr}}
    };
} // anonymous namespace


static bool CheckMdlLoadConfigFromFile(const aclmdlConfigHandle *handle)
{
    if (handle->attrState.find(ACL_MDL_PATH_PTR) == handle->attrState.end()) {
        ACL_LOG_INNER_ERROR("[Check][Type]model load type[%zu]: model path is not set in aclmdlConfigHandle",
            handle->mdlLoadType);
        return false;
    }
    return true;
}

static bool CheckMdlLoadConfigFromMem(const aclmdlConfigHandle *handle)
{
    if (handle->attrState.find(ACL_MDL_MEM_ADDR_PTR) == handle->attrState.end()) {
        ACL_LOG_INNER_ERROR("[Check][Type]model load type[%zu]: model memory ptr is not set in aclmdlConfigHandle",
            handle->mdlLoadType);
        return false;
    }

    if (handle->attrState.find(ACL_MDL_MEM_SIZET) == handle->attrState.end()) {
        ACL_LOG_INNER_ERROR("[Check][Type]model load type[%zu]: model memory size is not set in aclmdlConfigHandle",
            handle->mdlLoadType);
        return false;
    }
    return true;
}

static bool CheckMdlLoadConfigWithQ(const aclmdlConfigHandle *handle)
{
        if (handle->attrState.find(ACL_MDL_INPUTQ_ADDR_PTR) == handle->attrState.end()) {
            ACL_LOG_INNER_ERROR("[Check][Type]model load type[%zu]: inputQ ptr is not set in aclmdlConfigHandle",
                handle->mdlLoadType);
            return false;
        }

        if (handle->attrState.find(ACL_MDL_INPUTQ_NUM_SIZET) == handle->attrState.end()) {
            ACL_LOG_INNER_ERROR("[Check][Type]model load type[%zu]: inputQ num is not set in aclmdlConfigHandle",
                handle->mdlLoadType);
            return false;
        }

        if (handle->attrState.find(ACL_MDL_OUTPUTQ_ADDR_PTR) == handle->attrState.end()) {
            ACL_LOG_INNER_ERROR("[Check][Type]model load type[%zu]: outputQ ptr is not set in aclmdlConfigHandle",
                handle->mdlLoadType);
            return false;
        }

        if (handle->attrState.find(ACL_MDL_OUTPUTQ_NUM_SIZET) == handle->attrState.end()) {
            ACL_LOG_INNER_ERROR("[Check][Type]model load type[%zu]: outputQ num is not set in aclmdlConfigHandle",
                handle->mdlLoadType);
            return false;
        }

        return true;
}

bool CheckMdlConfigHandle(const aclmdlConfigHandle *handle)
{
    if (handle->attrState.find(ACL_MDL_LOAD_TYPE_SIZET) == handle->attrState.end()) {
        ACL_LOG_INNER_ERROR("[Find][Type]model load type is not set in aclmdlConfigHandle");
        return false;
    }

    if ((handle->mdlLoadType == static_cast<size_t>(ACL_MDL_LOAD_FROM_FILE)) ||
        (handle->mdlLoadType == static_cast<size_t>(ACL_MDL_LOAD_FROM_FILE_WITH_MEM))) {
        if (!CheckMdlLoadConfigFromFile(handle)) {
            return false;
        }
    }

    if ((handle->mdlLoadType == static_cast<size_t>(ACL_MDL_LOAD_FROM_MEM)) ||
        (handle->mdlLoadType == static_cast<size_t>(ACL_MDL_LOAD_FROM_MEM_WITH_MEM))) {
        if ((!CheckMdlLoadConfigFromMem(handle))) {
            return false;
        }
    }

    if (handle->mdlLoadType == static_cast<size_t>(ACL_MDL_LOAD_FROM_FILE_WITH_Q)) {
        if ((!CheckMdlLoadConfigFromFile(handle)) || (!CheckMdlLoadConfigWithQ(handle))) {
            return false;
        }
    }

    if (handle->mdlLoadType == static_cast<size_t>(ACL_MDL_LOAD_FROM_MEM_WITH_Q)) {
        if ((!CheckMdlLoadConfigFromMem(handle)) || (!CheckMdlLoadConfigWithQ(handle))) {
            return false;
        }
    }
    return true;
}

aclError aclmdlSetConfigOpt(aclmdlConfigHandle *handle, aclmdlConfigAttr attr,
    const void *attrValue, size_t valueSize)
{
    ACL_STAGES_REG(acl::ACL_STAGE_SET, acl::ACL_STAGE_DEFAULT);
    ACL_LOG_INFO("start to execute aclmdlSetConfigOpt, attr[%d]", static_cast<int32_t>(attr));
    ACL_REQUIRES_NOT_NULL_WITH_INPUT_REPORT(handle);
    ACL_REQUIRES_NOT_NULL_WITH_INPUT_REPORT(attrValue);
    SetMdlConfigParamFunc paramFunc;
    if (g_setMdlConfigMap.find(attr) != g_setMdlConfigMap.end()) {
        paramFunc = g_setMdlConfigMap[attr];
    } else {
        ACL_LOG_INNER_ERROR("[Find][Attr]attr[%d] is invalid. it should be [%d, %d]", static_cast<int32_t>(attr),
            static_cast<int32_t>(ACL_MDL_PRIORITY_INT32), static_cast<int32_t>(ACL_MDL_OUTPUTQ_ADDR_PTR));
        return ACL_ERROR_INVALID_PARAM;
    }
    aclError ret = paramFunc.checkFunc(attrValue, valueSize);
    if (ret != ACL_SUCCESS) {
        ACL_LOG_INNER_ERROR("[Check][Params]check params by aclmdlSetConfigOpt error, result[%d], attr[%d], "
            "valueSize[%zu]", ret, static_cast<int32_t>(attr), valueSize);
        return ret;
    }
    ret = paramFunc.setFunc(handle, attrValue);
    if (ret != ACL_SUCCESS) {
        ACL_LOG_INNER_ERROR("[Set][Params]set params by aclmdlSetConfigOpt error, result[%d], attr[%d]", ret,
            static_cast<int32_t>(attr));
        return ret;
    }

    ACL_LOG_INFO("successfully execute aclmdlSetConfigOpt, attr[%d]", static_cast<int32_t>(attr));
    return ACL_SUCCESS;
}

aclmdlConfigHandle *aclmdlCreateConfigHandle()
{
    ACL_STAGES_REG(acl::ACL_STAGE_CREATE, acl::ACL_STAGE_DEFAULT);
    ACL_ADD_APPLY_TOTAL_COUNT(ACL_STATISTICS_CREATE_DESTROY_MODEL_CONFIG);
    ACL_ADD_APPLY_SUCCESS_COUNT(ACL_STATISTICS_CREATE_DESTROY_MODEL_CONFIG);
    return new(std::nothrow) aclmdlConfigHandle();
}

aclError aclmdlDestroyConfigHandle(aclmdlConfigHandle *handle)
{
    ACL_STAGES_REG(acl::ACL_STAGE_DESTROY, acl::ACL_STAGE_DEFAULT);
    ACL_ADD_RELEASE_TOTAL_COUNT(ACL_STATISTICS_CREATE_DESTROY_MODEL_CONFIG);
    ACL_REQUIRES_NOT_NULL_WITH_INPUT_REPORT(handle);
    ACL_DELETE_AND_SET_NULL(handle);
    ACL_ADD_RELEASE_SUCCESS_COUNT(ACL_STATISTICS_CREATE_DESTROY_MODEL_CONFIG);
    return ACL_SUCCESS;
}