/**
* @file queue_process_host.cpp
*
* Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
#include "queue_process_host.h"
#include "log_inner.h"
#include "runtime/mem.h"
#include "runtime/rt_mem_queue.h"
#include "runtime/dev.h"
#include "aicpu/queue_schedule/qs_client.h"

namespace acl {
    aclError QueueProcessorHost::acltdtCreateQueue(const acltdtQueueAttr *attr, uint32_t *qid)
    {
        ACL_LOG_INFO("Start to acltdtCreateQueue");
        ACL_REQUIRES_NOT_NULL(qid);
        ACL_REQUIRES_NOT_NULL(attr);
        int32_t deviceId = 0;
        rtError_t rtRet = RT_ERROR_NONE;
        rtRet = rtGetDevice(&deviceId);
        if (rtRet != ACL_SUCCESS) {
            ACL_LOG_CALL_ERROR("[Get][DeviceId]fail to get deviceId result = %d", rtRet);
            return rtRet;
        }
        static bool isQueueIint = false;
        std::lock_guard<std::recursive_mutex> lock(muForQueueCtrl_);
        if (!isQueueIint) {
            ACL_LOG_INFO("need to init queue once");
            ACL_REQUIRES_CALL_RTS_OK(rtMemQueueInit(deviceId), rtMemQueueInit);
            isQueueIint = true;
        }
        ACL_REQUIRES_CALL_RTS_OK(rtMemQueueCreate(deviceId, attr, qid), rtMemQueueCreate);
        ACL_LOG_INFO("Successfully to execute acltdtCreateQueue, qid is %u", *qid);
        return ACL_SUCCESS;
    }

    aclError QueueProcessorHost::acltdtDestroyQueue(uint32_t qid)
    {
        int32_t deviceId = 0;
        ACL_REQUIRES_CALL_RTS_OK(rtGetDevice(&deviceId), rtGetDevice);
        // get qs id
        int32_t qsPid = 0;
        size_t routeNum = 0;
        std::lock_guard<std::recursive_mutex> lock(muForQueueCtrl_);
        if (GetDstInfo(deviceId, QS_PID, qsPid) == RT_ERROR_NONE) {
            rtEschedEventSummary_t eventSum = {0};
            rtEschedEventReply_t ack = {0};
            QsProcMsgRsp qsRsp = {0};
            int32_t cpPid = 0;
            ACL_REQUIRES_OK(GetDstInfo(deviceId, CP_PID, cpPid));
            eventSum.pid = cpPid;
            eventSum.grpId = 0;
            eventSum.eventId = RT_MQ_SCHED_EVENT_QS_MSG;
            eventSum.dstEngine = RT_MQ_DST_ENGINE_CCPU_DEVICE;
            ack.buf = reinterpret_cast<char *>(&qsRsp);
            ack.bufLen = sizeof(qsRsp);
            acltdtQueueRouteQueryInfo queryInfo = {BQS_QUERY_TYPE_SRC_OR_DST, qid, qid, true, true, true};
            ACL_REQUIRES_OK(GetQueueRouteNum(&queryInfo, deviceId, eventSum, ack, routeNum));
        }
        if (routeNum > 0) {
            ACL_LOG_ERROR("qid [%u] can not be destroyed, it need to be unbinded first.", qid);
            return ACL_ERROR_FAILURE;
        }
        ACL_REQUIRES_CALL_RTS_OK(rtMemQueueDestroy(deviceId, qid), rtMemQueueDestroy);
        DeleteMutexForData(qid);
        return ACL_SUCCESS;
    }

    aclError QueueProcessorHost::acltdtGrantQueue(uint32_t qid, int32_t pid, uint32_t permission, int32_t timeout)
    {
        ACL_LOG_INFO("start to acltdtGrantQueue, qid is %u, pid is %d, permisiion is %u, timeout is %d",
                     qid, pid, permission, timeout);
        if (permission & 0x1) {
            ACL_LOG_ERROR("[CHECK][permission]permission manager is not allowed");
            return ACL_ERROR_INVALID_PARAM;
        };
        int32_t deviceId = 0;
        ACL_REQUIRES_CALL_RTS_OK(rtGetDevice(&deviceId), rtGetDevice);
        uint64_t startTime = GetTimestamp();
        uint64_t endTime = 0;
        int32_t cpPid;
        rtBindHostpidInfo_t info = {0};
        info.hostPid = mmGetPid();
        info.cpType = RT_DEV_PROCESS_CP1;
        info.chipId = deviceId;
        bool continueFlag = (timeout < 0) ? true : false;
        do {
            if (rtQueryDevPid(&info, &cpPid) != RT_ERROR_NONE) {
                ACL_LOG_WARN("can not acquire cp pid, try again");
            } else {
                ACL_LOG_INFO("get cp pid %d", cpPid);
                break;
            }
            mmSleep(1); // sleep 1ms
            endTime = GetTimestamp();
            continueFlag = !continueFlag && ((endTime - startTime) <= (static_cast<uint64_t>(timeout) * MSEC_TO_USEC));
        } while (continueFlag);
        rtMemQueueShareAttr_t attr = {0};
        attr.manage = permission & ACL_TDT_QUEUE_PERMISSION_MANAGE;
        attr.read = permission & ACL_TDT_QUEUE_PERMISSION_DEQUEUE;
        attr.write = permission & ACL_TDT_QUEUE_PERMISSION_ENQUEUE;
        std::lock_guard<std::recursive_mutex> lock(muForQueueCtrl_);
        ACL_REQUIRES_CALL_RTS_OK(rtMemQueueGrant(deviceId, qid, pid, &attr), rtMemQueueGrant);
        ACL_LOG_INFO("successfully execute acltdtGrantQueue, qid is %u, pid is %d, permisiion is %u, timeout is %d",
                     qid, pid, permission, timeout);
        return ACL_SUCCESS;
    }

    aclError QueueProcessorHost::acltdtAttachQueue(uint32_t qid, int32_t timeout, uint32_t *permission)
    {
        ACL_LOG_INFO("start to acltdtGrantQueue, qid is %u, permisiion is %u, timeout is %d",
                     qid, *permission, timeout);
        ACL_REQUIRES_NOT_NULL(permission);
        int32_t deviceId = 0;
        ACL_REQUIRES_CALL_RTS_OK(rtGetDevice(&deviceId), rtGetDevice);
        std::lock_guard<std::recursive_mutex> lock(muForQueueCtrl_);
        ACL_REQUIRES_CALL_RTS_OK(rtMemQueueAttach(deviceId, qid, timeout * MSEC_TO_USEC), rtMemQueueAttach);
        ACL_LOG_INFO("start to query qid %u permisiion", qid);
        rtMemQueueShareAttr_t attr = {0};
        ACL_REQUIRES_CALL_RTS_OK(GetQueuePermission(deviceId, qid, attr), GetQueuePermission);
        uint32_t tmp = 0;
        tmp = attr.manage ? (tmp | ACL_TDT_QUEUE_PERMISSION_MANAGE) : tmp;
        tmp = attr.read ? (tmp | ACL_TDT_QUEUE_PERMISSION_DEQUEUE) : tmp;
        tmp = attr.write ? (tmp | ACL_TDT_QUEUE_PERMISSION_ENQUEUE) : tmp;
        *permission = tmp;
        ACL_LOG_INFO("successfully execute to get queue %u permission %u", qid, *permission);
        ACL_LOG_INFO("successfully execute acltdtGrantQueue, qid is %u, permisiion is %u, timeout is %d",
                     qid, *permission, timeout);
        return ACL_SUCCESS;
    }

    aclError QueueProcessorHost::acltdtBindQueueRoutes(acltdtQueueRouteList *qRouteList)
    {
        ACL_REQUIRES_NOT_NULL(qRouteList);
        ACL_LOG_INFO("Start to acltdtBindQueueRoutes, queue route is %zu", qRouteList->routeList.size());
        int32_t deviceId = 0;
        ACL_REQUIRES_CALL_RTS_OK(rtGetDevice(&deviceId), rtGetDevice);
        ACL_REQUIRES_OK(InitQueueSchedule(deviceId));
        // get dst pid
        int32_t dstPid = 0;
        ACL_REQUIRES_OK(GetDstInfo(deviceId, CP_PID, dstPid));
        rtEschedEventSummary_t eventSum = {0};
        rtEschedEventReply_t ack = {0};
        QsProcMsgRsp qsRsp = {0};
        eventSum.pid = dstPid;
        eventSum.grpId = 0;
        eventSum.eventId = RT_MQ_SCHED_EVENT_QS_MSG;
        eventSum.dstEngine = RT_MQ_DST_ENGINE_CCPU_DEVICE;
        ack.buf = reinterpret_cast<char *>(&qsRsp);
        ack.bufLen = sizeof(qsRsp);
        std::lock_guard<std::recursive_mutex> lock(muForQueueCtrl_);
        if (!isQsInit_) {
            ACL_REQUIRES_OK(SendConnectQsMsg(deviceId, eventSum, ack));
            isQsInit_ = true;
        }
        ACL_REQUIRES_OK(SendBindUnbindMsg(qRouteList, deviceId, true, false, eventSum, ack));
        ACL_LOG_INFO("Successfully to execute acltdtBindQueueRoutes, queue route is %zu", qRouteList->routeList.size());
        return ACL_SUCCESS;
    }

    aclError QueueProcessorHost::acltdtUnbindQueueRoutes(acltdtQueueRouteList *qRouteList)
    {
        ACL_REQUIRES_NOT_NULL(qRouteList);
        ACL_LOG_INFO("Start to acltdtUnBindQueueRoutes, queue route is %zu", qRouteList->routeList.size());
        int32_t deviceId = 0;
        ACL_REQUIRES_CALL_RTS_OK(rtGetDevice(&deviceId), rtGetDevice);
        // get dst pid
        int32_t dstPid = 0;
        ACL_REQUIRES_OK(GetDstInfo(deviceId, CP_PID, dstPid));
        rtEschedEventSummary_t eventSum = {0};
        rtEschedEventReply_t ack = {0};
        QsProcMsgRsp qsRsp = {0};
        eventSum.pid = dstPid;
        eventSum.grpId = 0;
        eventSum.eventId = RT_MQ_SCHED_EVENT_QS_MSG;
        eventSum.dstEngine = RT_MQ_DST_ENGINE_CCPU_DEVICE;
        ack.buf = reinterpret_cast<char *>(&qsRsp);
        ack.bufLen = sizeof(qsRsp);
        std::lock_guard<std::recursive_mutex> lock(muForQueueCtrl_);
        ACL_REQUIRES_OK(SendBindUnbindMsg(qRouteList, deviceId, false, false, eventSum, ack));
        ACL_LOG_INFO("Successfully to execute acltdtUnBindQueueRoutes, queue route is %zu",
                     qRouteList->routeList.size());
        return ACL_SUCCESS;
    }

    aclError QueueProcessorHost::acltdtQueryQueueRoutes(const acltdtQueueRouteQueryInfo *queryInfo,
                                                        acltdtQueueRouteList *qRouteList)
    {
        ACL_REQUIRES_NOT_NULL(queryInfo);
        ACL_REQUIRES_NOT_NULL(qRouteList);
        ACL_LOG_INFO("Start to acltdtQueryQueueRoutes");
        int32_t deviceId = 0;
        ACL_REQUIRES_CALL_RTS_OK(rtGetDevice(&deviceId), rtGetDevice);
        // get dst id
        int32_t dstPid = 0;
        ACL_REQUIRES_OK(GetDstInfo(deviceId, CP_PID, dstPid));
        rtEschedEventSummary_t eventSum = {0};
        rtEschedEventReply_t ack = {0};
        QsProcMsgRsp qsRsp = {0};
        eventSum.pid = dstPid;
        eventSum.grpId = 0;
        eventSum.eventId = RT_MQ_SCHED_EVENT_QS_MSG;
        eventSum.dstEngine = RT_MQ_DST_ENGINE_CCPU_DEVICE;
        ack.buf = reinterpret_cast<char *>(&qsRsp);
        ack.bufLen = sizeof(qsRsp);
        std::lock_guard<std::recursive_mutex> lock(muForQueueCtrl_);
        size_t routeNum = 0;
        ACL_REQUIRES_OK(GetQueueRouteNum(queryInfo, deviceId, eventSum, ack, routeNum));
        ACL_REQUIRES_OK(QueryQueueRoutes(queryInfo, deviceId, false, routeNum, eventSum, ack, qRouteList));
        return ACL_SUCCESS;
    }
}