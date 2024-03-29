/**
* @file queue_manager.cpp
*
* Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "log_inner.h"
#include "queue.h"
#include "queue_manager.h"
#include "queue_process.h"
#include "queue_process_host.h"
#include "queue_process_mdc.h"
#include "queue_process_ccpu.h"

namespace acl {
    QueueManager& QueueManager::GetInstance()
    {
        static QueueManager instance;
        (void)GetRunningEnv(instance.env_);
        return instance;
    }

    aclError QueueManager::GetRunningEnv(RunEnv &runningEnv)
    {
        // get acl run mode
        if (runningEnv != ACL_ACL_ENV_UNKNOWN) {
            return ACL_SUCCESS;
        }
        aclrtRunMode aclRunMode;
        aclError getRunModeRet = aclrtGetRunMode(&aclRunMode);
        if (getRunModeRet != ACL_SUCCESS) {
            ACL_LOG_CALL_ERROR("[Get][RunMode]get run mode failed, result = %d.", getRunModeRet);
            return getRunModeRet;
        }
        if (aclRunMode == ACL_HOST) {
            runningEnv = ACL_ENV_HOST;
        } else if (aclRunMode == ACL_DEVICE) {
            // get env config
            const char *sharePoolPreConfig = std::getenv("SHAREGROUP_PRECONFIG");
            if (sharePoolPreConfig == nullptr) {
                ACL_LOG_INFO("This is not share group preconfig");
                runningEnv = ACL_ENV_DEVICE_CCPU;
            } else {
                ACL_LOG_INFO("This is share group preconfig");
                runningEnv = ACL_ENV_DEVICE_MDC;
            }
        } else {
            ACL_LOG_INNER_ERROR("[Get][RunMode]get run mode failed, result = %d.", getRunModeRet);
            return ACL_ERROR_FAILURE;
        }
        return ACL_SUCCESS;
    }

    QueueProcessor *QueueManager::GetQueueProcessor()
    {
        if (queueProcessProc_ != nullptr) {
            return queueProcessProc_.get();
        }
        try {
            switch(env_) {
                case ACL_ENV_HOST:
                    queueProcessProc_ = std::unique_ptr<QueueProcessorHost>(new (std::nothrow)QueueProcessorHost());
                    break;
                case ACL_ENV_DEVICE_MDC:
                    queueProcessProc_ = std::unique_ptr<QueueProcessorMdc>(new (std::nothrow)QueueProcessorMdc());
                    break;
                case ACL_ENV_DEVICE_CCPU:
                    queueProcessProc_ = std::unique_ptr<QueueProcessorCcpu>(new (std::nothrow)QueueProcessorCcpu());
                    break;
                default:
                    ACL_LOG_INNER_ERROR("[Check][Runenv]check runenv failed.");
                    return nullptr;
            }
        } catch (...) {
            ACL_LOG_INNER_ERROR("[Define][Object]define object queue processor with unique_ptr failed.");
            return nullptr;
        }
        return queueProcessProc_.get();
    }
}
