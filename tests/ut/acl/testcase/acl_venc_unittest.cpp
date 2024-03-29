#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#define protected public
#define private public
#include "acl/ops/acl_dvpp.h"
#include "single_op/op_executor.h"
#include "single_op/dvpp/common/dvpp_def_internal.h"
#include "single_op/dvpp/common/dvpp_util.h"
#include "single_op/dvpp/base/video_processor.h"
#include "single_op/dvpp/v200/video_processor_v200.h"
#include "single_op/dvpp/v100/video_processor_v100.h"
#include "single_op/dvpp/mgr/dvpp_manager.h"
#undef private
#undef protected

#include "acl/acl.h"
#include "runtime/rt.h"
#include "acl_stub.h"

using namespace std;
using namespace testing;
using namespace acl;
using namespace acl::dvpp;

void VencCallbackStub(acldvppPicDesc *input, acldvppStreamDesc *output, void *userdata)
{

}

class VencTest : public testing::Test {
protected:

    void SetUp()
    {
        acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
        dvppManager.dvppVersion_ = DVPP_KERNELS_V100;
        dvppManager.aclRunMode_ = ACL_HOST;
        dvppManager.InitDvppProcessor();
    }

    void TearDown()
    {
        Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));
    }
};

void* mmAlignMallocStubDvpp(mmSize mallocSize, mmSize alignSize)
{
    aclvdecChannelDesc *aclChannelDesc = nullptr;
    mallocSize = CalAclDvppStructSize(aclChannelDesc);
    return malloc(mallocSize);
}

TEST_F(VencTest, aclvencCreateChannel_0)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));

    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
    dvppManager.dvppVersion_ = DVPP_KERNELS_V200;
    dvppManager.aclRunMode_ = ACL_HOST;
    dvppManager.InitDvppProcessor();
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();

    vencChannelDesc->vencDesc.len = 10;
    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_EQ(ret, ACL_SUCCESS);

    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_2)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtGetDevice(_))
        .WillRepeatedly(Return((ACL_ERROR_RT_PARAM_INVALID)));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_3)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyCreate(_, _))
        .WillOnce(Return((1)));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_4)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyCreate(_, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return((1)));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_5)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtGetNotifyID(_, _))
        .WillOnce(Return(1));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_6)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtGetNotifyID(_, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return((1)));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_7)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamCreate(_, _))
        .WillOnce(Return(1));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_8)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamCreate(_, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return(1));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_9)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpyAsync(_, _, _, _, _, _))
        .WillOnce(Return((1)));

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_10)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCpuKernelLaunch(_, _, _, _, _, _, _))
        .WillOnce(Return(1));
    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_12)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtSubscribeReport(_,_))
        .WillOnce(Return((1)));
    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_13)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtSubscribeReport(_,_))
        .WillOnce(Return((1)));
    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_14)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMalloc(_, _, _))
        .WillOnce(Return((1)));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencCreateChannel_15)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMalloc(_, _, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return((1)));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();

    aclError ret = aclvencCreateChannel(vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencDestroyChannel_0)
{
    aclvencChannelDesc *vencChannelDesc = nullptr;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtFree(_))
        .WillRepeatedly(Return((1)));
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencSetGet_0)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    aclvencGetChannelDescChannelId(vencChannelDesc);

    aclError ret = aclvencSetChannelDescThreadId(vencChannelDesc,1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetChannelDescThreadId(vencChannelDesc);

    ret = aclvencSetChannelDescCallback(vencChannelDesc,VencCallbackStub);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetChannelDescCallback(vencChannelDesc);

    ret = aclvencSetChannelDescEnType(vencChannelDesc, H265_MAIN_LEVEL);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetChannelDescEnType(vencChannelDesc);

    ret = aclvencSetChannelDescPicFormat(vencChannelDesc, PIXEL_FORMAT_YUV_SEMIPLANAR_420);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetChannelDescPicFormat(vencChannelDesc);

    ret = aclvencSetChannelDescPicFormat(vencChannelDesc, PIXEL_FORMAT_YUV_400);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    ret = aclvencSetChannelDescPicWidth(vencChannelDesc,0);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetChannelDescPicWidth(vencChannelDesc);

    ret = aclvencSetChannelDescPicHeight(vencChannelDesc,0);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetChannelDescPicHeight(vencChannelDesc);

    ret = aclvencSetChannelDescKeyFrameInterval(vencChannelDesc,16);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetChannelDescKeyFrameInterval(vencChannelDesc);

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    ret = aclvencSetFrameConfigEos(config, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetFrameConfigEos(config);
    ret = aclvencSetFrameConfigForceIFrame(config, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetFrameConfigForceIFrame(config);

    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSetGet_1)
{
    aclvencChannelDesc *vencChannelDesc = nullptr;
    aclvencGetChannelDescChannelId(vencChannelDesc);

    aclError ret = aclvencSetChannelDescThreadId(vencChannelDesc,1);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencGetChannelDescThreadId(vencChannelDesc);

    ret = aclvencSetChannelDescCallback(vencChannelDesc,VencCallbackStub);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencGetChannelDescCallback(vencChannelDesc);

    ret = aclvencSetChannelDescEnType(vencChannelDesc, H265_MAIN_LEVEL);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencGetChannelDescEnType(vencChannelDesc);

    ret = aclvencSetChannelDescPicFormat(vencChannelDesc, PIXEL_FORMAT_YUV_SEMIPLANAR_420);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencGetChannelDescPicFormat(vencChannelDesc);

    ret = aclvencSetChannelDescPicWidth(vencChannelDesc,0);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencGetChannelDescPicWidth(vencChannelDesc);

    ret = aclvencSetChannelDescPicHeight(vencChannelDesc,0);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencGetChannelDescPicHeight(vencChannelDesc);

    ret = aclvencSetChannelDescKeyFrameInterval(vencChannelDesc,16);
    EXPECT_NE(ret, ACL_SUCCESS);
    aclvencGetChannelDescKeyFrameInterval(vencChannelDesc);

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    ret = aclvencSetFrameConfigEos(config, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetFrameConfigEos(config);
    ret = aclvencSetFrameConfigForceIFrame(config, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetFrameConfigForceIFrame(config);

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSetGetAddrSize)
{
    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
    dvppManager.dvppVersion_ = DVPP_KERNELS_V200;
    dvppManager.aclRunMode_ = ACL_HOST;
    dvppManager.InitDvppProcessor();

    aclvencChannelDesc *vencChannelDesc = nullptr;
    int32_t ret = aclvencSetChannelDescBufAddr(vencChannelDesc, nullptr);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    aclvencChannelDesc channel;
    ret = aclvencSetChannelDescBufAddr(&channel, nullptr);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    int32_t addr = 10;
    ret = aclvencSetChannelDescBufAddr(&channel, &addr);
    EXPECT_EQ(ret, ACL_SUCCESS);

    ret = aclvencSetChannelDescBufAddr(&channel, &addr);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    void *address = aclvencGetChannelDescBufAddr(nullptr);
    EXPECT_EQ(address, nullptr);

    address = aclvencGetChannelDescBufAddr(&channel);
    EXPECT_NE(address, nullptr);

    ret = aclvencSetChannelDescBufSize(vencChannelDesc, 5 * 1024 * 1024 * sizeof(int32_t));
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    ret = aclvencSetChannelDescBufSize(&channel, sizeof(int32_t));
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    ret = aclvencSetChannelDescBufSize(&channel, 5 * 1024 * 1024 * sizeof(int32_t));
    EXPECT_EQ(ret, ACL_SUCCESS);

    int32_t size = aclvencGetChannelDescBufSize(&channel);
    EXPECT_EQ(size, 5 * 1024 * 1024 * sizeof(int32_t));

    size = aclvencGetChannelDescBufSize(nullptr);
    EXPECT_EQ(size, 0);
}

TEST_F(VencTest, aclvencSetGetAddrSizeNotSupported)
{
    aclvencChannelDesc channel;
    auto ret = aclvencSetChannelDescBufAddr(&channel, nullptr);
    EXPECT_EQ(ret, ACL_ERROR_FEATURE_UNSUPPORTED);

    auto address = aclvencGetChannelDescBufAddr(&channel);
    EXPECT_EQ(address, nullptr);

    ret = aclvencSetChannelDescBufSize(&channel, sizeof(int32_t));
    EXPECT_EQ(ret, ACL_ERROR_FEATURE_UNSUPPORTED);

    int32_t size = aclvencGetChannelDescBufSize(&channel);
    EXPECT_EQ(size, 0);
}

TEST_F(VencTest, aclvencSetGetRateCtrlPara)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *channel = aclvencCreateChannelDesc();
    auto ret = aclvencSetChannelDescRcMode(channel, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);

    auto rcMode = aclvencGetChannelDescRcMode(channel);
    EXPECT_EQ(rcMode, 1);

    rcMode = aclvencGetChannelDescRcMode(nullptr);
    EXPECT_EQ(rcMode, 0);

    ret = aclvencSetChannelDescSrcRate(channel, 2);
    EXPECT_EQ(ret, ACL_SUCCESS);

    auto srcRate = aclvencGetChannelDescSrcRate(channel);
    EXPECT_EQ(srcRate, 2);

    srcRate = aclvencGetChannelDescSrcRate(nullptr);
    EXPECT_EQ(srcRate, 0);

    ret = aclvencSetChannelDescMaxBitRate(channel, 3);
    EXPECT_EQ(ret, ACL_SUCCESS);

    auto maxBitRate = aclvencGetChannelDescMaxBitRate(channel);
    EXPECT_EQ(maxBitRate, 3);

    maxBitRate = aclvencGetChannelDescMaxBitRate(nullptr);
    EXPECT_EQ(maxBitRate, 0);

    aclvencDestroyChannelDesc(channel);
}

TEST_F(VencTest, aclvencSetGetRateCtrlParaNotSupported)
{
    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
    dvppManager.dvppVersion_ = DVPP_KERNELS_V200;
    dvppManager.aclRunMode_ = ACL_HOST;
    dvppManager.InitDvppProcessor();
    aclvencChannelDesc channel;

    auto ret = aclvencSetChannelDescRcMode(&channel, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);

    auto rcMode = aclvencGetChannelDescRcMode(&channel);
    EXPECT_EQ(rcMode, 1);

    ret = aclvencSetChannelDescRcMode(&channel, 3);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    ret = aclvencSetChannelDescSrcRate(&channel, 2);
    EXPECT_EQ(ret, ACL_SUCCESS);

    auto srcRate = aclvencGetChannelDescSrcRate(&channel);
    EXPECT_EQ(srcRate, 2);

    ret = aclvencSetChannelDescMaxBitRate(&channel, 3);
    EXPECT_EQ(ret, ACL_SUCCESS);

    auto maxBitRate = aclvencGetChannelDescMaxBitRate(&channel);
    EXPECT_EQ(maxBitRate, 3);
}

TEST_F(VencTest, aclvencSetGetVencParam)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *desc = aclvencCreateChannelDesc();
    desc->dataBuffer.data = reinterpret_cast<void *>(0x1);
    uint64_t threadId = 100;
    auto ret = aclvencSetChannelDescParam(desc, ACL_VENC_THREAD_ID_UINT64, 8, &threadId);
    EXPECT_EQ(ret, ACL_SUCCESS);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    uint64_t threadId1 = 0;
    size_t threadIdLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_THREAD_ID_UINT64, 8, &threadIdLen, &threadId1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(threadId1, 100);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_THREAD_ID_UINT64, 9, &threadId);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_THREAD_ID_UINT64, 7, &threadIdLen, &threadId1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    aclvencCallback callback = (aclvencCallback)0x1;
    void *func = (void *)callback;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_CALLBACK_PTR, 8, &func);
    EXPECT_EQ(ret, ACL_SUCCESS);
    void *func1 = nullptr;
    size_t funcLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_CALLBACK_PTR, 8, &funcLen, &func1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(func1, (void *)0x1);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_CALLBACK_PTR, 9, &func);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_CALLBACK_PTR, 7, &funcLen, &func1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    acldvppPixelFormat picFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_PIXEL_FORMAT_UINT32, 4, &picFormat);
    EXPECT_EQ(ret, ACL_SUCCESS);
    acldvppPixelFormat picFormat1 = PIXEL_FORMAT_YUV_400;
    size_t picFormatLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_PIXEL_FORMAT_UINT32, 4, &picFormatLen, &picFormat1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(picFormat1, PIXEL_FORMAT_YUV_SEMIPLANAR_420);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_PIXEL_FORMAT_UINT32, 5, &picFormat);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_PIXEL_FORMAT_UINT32, 3, &picFormatLen, &picFormat1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    acldvppStreamFormat enType = H265_MAIN_LEVEL;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_ENCODE_TYPE_UINT32, 4, &enType);
    EXPECT_EQ(ret, ACL_SUCCESS);
    acldvppStreamFormat enType1 = H264_MAIN_LEVEL;
    size_t enTypeLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_ENCODE_TYPE_UINT32, 4, &enTypeLen, &enType1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(enType1, H265_MAIN_LEVEL);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_ENCODE_TYPE_UINT32, 5, &enType);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_ENCODE_TYPE_UINT32, 3, &enTypeLen, &enType1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t picWidth = 100;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_PIC_WIDTH_UINT32, 4, &picWidth);
    EXPECT_EQ(ret, ACL_SUCCESS);
    uint32_t picWidth1 = 0;
    size_t widthLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_PIC_WIDTH_UINT32, 4, &widthLen, &picWidth1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(picWidth1, 100);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_PIC_WIDTH_UINT32, 5, &picWidth);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_PIC_WIDTH_UINT32, 3, &widthLen, &picWidth1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t picHeight = 100;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_PIC_HEIGHT_UINT32, 4, &picHeight);
    EXPECT_EQ(ret, ACL_SUCCESS);
    uint32_t picHeight1 = 0;
    size_t heightLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_PIC_HEIGHT_UINT32, 4, &heightLen, &picHeight1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(picHeight1, 100);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_PIC_HEIGHT_UINT32, 5, &picHeight);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_PIC_HEIGHT_UINT32, 3, &heightLen, &picHeight1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t keyFrameInterval = 100;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_KEY_FRAME_INTERVAL_UINT32, 4, &keyFrameInterval);
    EXPECT_EQ(ret, ACL_SUCCESS);
    uint32_t keyFrameInterval1 = 0;
    size_t keyFrameIntervalLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_KEY_FRAME_INTERVAL_UINT32, 4, &keyFrameIntervalLen,
        &keyFrameInterval1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(keyFrameInterval1, 100);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_KEY_FRAME_INTERVAL_UINT32, 5, &keyFrameInterval);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_KEY_FRAME_INTERVAL_UINT32, 3, &keyFrameIntervalLen,
        &keyFrameInterval1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    void *buf = (void *)0x1;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_BUF_ADDR_PTR, 8, &buf);
    EXPECT_EQ(ret, ACL_ERROR_FEATURE_UNSUPPORTED);
    void *buf1 = nullptr;
    size_t bufLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_BUF_ADDR_PTR, 8, &bufLen, &buf1);
    EXPECT_EQ(ret, ACL_ERROR_FEATURE_UNSUPPORTED);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_BUF_ADDR_PTR, 9, &buf);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t bufSize = 100;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_BUF_SIZE_UINT32, 4, &bufSize);
    EXPECT_EQ(ret, ACL_ERROR_FEATURE_UNSUPPORTED);
    uint32_t bufSize1 = 0;
    size_t bufSizeLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_BUF_SIZE_UINT32, 4, &bufSizeLen, &bufSize1);
    EXPECT_EQ(ret, ACL_ERROR_FEATURE_UNSUPPORTED);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_BUF_SIZE_UINT32, 5, &bufSize);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t rcMode = 1;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_RC_MODE_UINT32, 4, &rcMode);
    EXPECT_EQ(ret, ACL_SUCCESS);
    uint32_t rcMode1 = 0;
    size_t rcModeLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_RC_MODE_UINT32, 4, &rcModeLen, &rcMode1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(rcMode1, 1);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_RC_MODE_UINT32, 5, &rcMode);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_RC_MODE_UINT32, 3, &rcModeLen, &rcMode1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t srcRate = 100;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_SRC_RATE_UINT32, 4, &srcRate);
    EXPECT_EQ(ret, ACL_SUCCESS);
    uint32_t srcRate1 = 0;
    size_t srcRateLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_SRC_RATE_UINT32, 4, &srcRateLen, &srcRate1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(srcRate1, 100);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_SRC_RATE_UINT32, 5, &srcRate);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_SRC_RATE_UINT32, 3, &srcRateLen, &srcRate1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t maxBitRate = 100;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_MAX_BITRATE_UINT32, 4, &maxBitRate);
    EXPECT_EQ(ret, ACL_SUCCESS);
    uint32_t maxBitRate1 = 0;
    size_t maxBitRateLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_MAX_BITRATE_UINT32, 4, &maxBitRateLen, &maxBitRate1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(maxBitRate1, 100);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_MAX_BITRATE_UINT32, 5, &maxBitRate);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_MAX_BITRATE_UINT32, 3, &maxBitRateLen, &maxBitRate1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    uint32_t ipProp = 100;
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_MAX_IP_PROP_UINT32, 4, &ipProp);
    EXPECT_EQ(ret, ACL_SUCCESS);
    uint32_t ipProp1 = 0;
    size_t ipPropLen = 0;
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_MAX_IP_PROP_UINT32, 4, &ipPropLen, &ipProp1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    EXPECT_EQ(ipProp1, 100);
    ret = aclvencSetChannelDescParam(desc, ACL_VENC_MAX_IP_PROP_UINT32, 5, &ipProp);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);
    ret = aclvencGetChannelDescParam(desc, ACL_VENC_MAX_IP_PROP_UINT32, 3, &ipPropLen, &ipProp1);
    EXPECT_EQ(ret, ACL_ERROR_INVALID_PARAM);

    aclvencDestroyChannelDesc(desc);
}

TEST_F(VencTest, aclvencSendFrame_DestroyFrameConfig_fail)
{
    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclError ret = aclvencDestroyFrameConfig(config);
    EXPECT_EQ(ret, ACL_SUCCESS);
}

TEST_F(VencTest, aclvencSendFrame_0)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();
    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclError ret = aclvencSetFrameConfigEos(config, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetFrameConfigEos(config);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyWait(_, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return(1));

    acldvppStreamDesc *outStreamDesc = nullptr;
    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_EQ(ret, ACL_SUCCESS);

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_1)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;

    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();
    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclError ret = aclvencSetFrameConfigEos(config, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);
    aclvencGetFrameConfigEos(config);

    acldvppStreamDesc *outStreamDesc = nullptr;
    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_EQ(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_2)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclError ret = aclvencSetFrameConfigEos(config, 1);
    EXPECT_EQ(ret, ACL_SUCCESS);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpy(_, _, _, _, _))
        .WillRepeatedly(Return((1)));

    acldvppStreamDesc *outStreamDesc = nullptr;
    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_EQ(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_3)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 1);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCpuKernelLaunch(_, _, _, _, _, _, _))
        .WillOnce(Return(1));

    acldvppStreamDesc *outStreamDesc = nullptr;
    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_4)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 1);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamSynchronize(_))
        .WillRepeatedly(Return(1));

    acldvppStreamDesc *outStreamDesc = nullptr;
    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

rtError_t rtCallbackLaunchStubCallBackVencOk(rtCallback_t callBackFunc, void *fnData, rtStream_t stream, bool isBlock)
{
    callBackFunc(fnData);
    return RT_ERROR_NONE;
}

TEST_F(VencTest, aclvencSendFrame_6)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);

    acldvppStreamDesc *outStreamDesc = nullptr;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));
    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_EQ(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_7)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    vencChannelDesc->outputMemMode = 1;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);

    acldvppStreamDesc *outStreamDesc = acldvppCreateStreamDesc();//nullptr;

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;

    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_EQ(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_8)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    vencChannelDesc->outputMemMode = 2;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);

    acldvppStreamDesc *outStreamDesc = nullptr;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_9)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpy(_, _, _, _, _))
        .WillOnce(Return((1)));

    acldvppStreamDesc *outStreamDesc = nullptr;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_10)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpy(_, _, _, _, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return(1));
    acldvppStreamDesc *outStreamDesc = nullptr;

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_11)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyWait(_, _))
        .WillOnce(Return((1)));

    acldvppStreamDesc *outStreamDesc = nullptr;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_12)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyWait(_, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return(1));
    acldvppStreamDesc *outStreamDesc = nullptr;

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;

    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_13)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamSynchronize(_))
        .WillOnce(Return((1)));

    acldvppStreamDesc *outStreamDesc = nullptr;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_14)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    vencChannelDesc->callback = VencCallbackStub;
    vencChannelDesc->outputMemMode = 1;
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 1);
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamSynchronize(_))
        .WillRepeatedly(Return(1));

    acldvppStreamDesc *outStreamDesc = nullptr;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillRepeatedly(Invoke((rtCallbackLaunchStubCallBackVencOk)));

    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;

    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyStreamDesc(outStreamDesc);
    outStreamDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencSendFrame_15)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->callback = VencCallbackStub;
    vencChannelDesc->sendFrameStream = malloc(4);
    vencChannelDesc->getFrameStream = malloc(4);
    acldvppPicDesc *inputPicDesc = acldvppCreatePicDesc();

    aclvencFrameConfig *config = aclvencCreateFrameConfig();
    aclvencSetFrameConfigEos(config, 0);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCpuKernelLaunch(_, _, _, _, _, _,_))
        .WillOnce(Return((1)));

    acldvppStreamDesc *outStreamDesc = nullptr;
    vencChannelDesc->vencDesc.picWidth = 2;
    vencChannelDesc->vencDesc.picHeight = 2;
    inputPicDesc->dvppPicDesc.size = 6;
    aclError ret = aclvencSendFrame(vencChannelDesc, inputPicDesc, outStreamDesc, config, nullptr);
    EXPECT_NE(ret, ACL_SUCCESS);

    free(vencChannelDesc->sendFrameStream);
    free(vencChannelDesc->getFrameStream);
    aclvencDestroyChannelDesc(vencChannelDesc);
    vencChannelDesc = nullptr;

    acldvppDestroyPicDesc(inputPicDesc);
    inputPicDesc = nullptr;

    aclvencDestroyFrameConfig(config);
    config = nullptr;
}

TEST_F(VencTest, aclvencV200_CreateDestroyChannelDescHost)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
    dvppManager.dvppVersion_ = DVPP_KERNELS_V200;
    dvppManager.aclRunMode_ = ACL_HOST;
    dvppManager.InitDvppProcessor();

    aclvencChannelDesc *desc = aclvencCreateChannelDesc();
    EXPECT_NE(desc, nullptr);

    int32_t ret = aclvencDestroyChannelDesc(desc);
    EXPECT_EQ(ret, ACL_SUCCESS);
}

TEST_F(VencTest, aclvencV200_CreateDestroyChannelDescDevice)
{
    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
    dvppManager.dvppVersion_ = DVPP_KERNELS_V200;
    dvppManager.aclRunMode_ = ACL_DEVICE;
    dvppManager.InitDvppProcessor();

    aclvencChannelDesc *desc = aclvencCreateChannelDesc();
    EXPECT_NE(desc, nullptr);

    int32_t ret = aclvencDestroyChannelDesc(desc);
    EXPECT_EQ(ret, ACL_SUCCESS);
}

TEST_F(VencTest, aclvencV200_MallocOutMemoryFailed1)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
    dvppManager.dvppVersion_ = DVPP_KERNELS_V200;
    dvppManager.aclRunMode_ = ACL_HOST;
    dvppManager.InitDvppProcessor();

    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    vencChannelDesc->bufSize = 1;
    vencChannelDesc->bufAddr = 1;
    vencChannelDesc->outputMemMode = 2; // VENC_USER_MODE

    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);
    aclError ret = v200.aclvencMallocOutMemory(vencChannelDesc);
    EXPECT_EQ(ret, ACL_SUCCESS);
    v200.aclvencFreeOutMemory(vencChannelDesc);
    ret = aclvencDestroyChannelDesc(vencChannelDesc);
    EXPECT_EQ(ret, ACL_SUCCESS);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, aclvencV200_MallocOutMemoryFailed2)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();
    dvppManager.dvppVersion_ = DVPP_KERNELS_V200;
    dvppManager.aclRunMode_ = ACL_HOST;
    dvppManager.InitDvppProcessor();
    dvpp::VideoProcessor processor(ACL_HOST);

    aclvencChannelDesc *vencChannelDesc = aclvencCreateChannelDesc();
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMalloc(_, _, _))
        .WillOnce(Return(RT_ERROR_NONE));
    vencChannelDesc->bufAddr = 0;
    vencChannelDesc->bufSize == 0;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtDvppMalloc(_, _))
        .WillOnce(Return(ACL_ERROR_RT_PARAM_INVALID))
        .WillRepeatedly(Return(RT_ERROR_NONE));
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);
    aclError ret = v200.aclvencMallocOutMemory(vencChannelDesc);
    EXPECT_EQ(ret, ACL_ERROR_RT_PARAM_INVALID);
    v200.aclvencFreeOutMemory(vencChannelDesc);

    ret = aclvencDestroyChannelDesc(vencChannelDesc);
    EXPECT_EQ(ret, ACL_SUCCESS);
    vencChannelDesc = nullptr;
}

TEST_F(VencTest, CreateEventForVencChannelTest)
{
    aclvencChannelDesc vencChannelDesc;
    vencChannelDesc.dataBuffer.data = reinterpret_cast<void *>(0x1);
    bool isNeedNotify;
    char *kernelName;
    auto imageProcessor = acl::dvpp::DvppManager::GetInstance().GetVideoProcessor();
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventCreateWithFlag(_, _))
        .WillOnce(Return((1)));
    aclError ret = imageProcessor->CreateEventForVencChannel(&vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventCreateWithFlag(_, _))
        .WillOnce(Return(0))
        .WillRepeatedly(Return(1));
    ret = imageProcessor->CreateEventForVencChannel(&vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventCreateWithFlag(_, _))
        .WillOnce(Return(0))
        .WillRepeatedly(Return(0));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtGetEventID(_, _))
        .WillOnce(Return((1)));
    ret = imageProcessor->CreateEventForVencChannel(&vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventCreateWithFlag(_, _))
        .WillOnce(Return(0))
        .WillRepeatedly(Return(0));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtGetEventID(_, _))
        .WillOnce(Return(0))
        .WillRepeatedly(Return(1));
    ret = imageProcessor->CreateEventForVencChannel(&vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
}

TEST_F(VencTest, CreateNotifyAndStremForVencChannelTest)
{
    aclvencChannelDesc *channelDesc = nullptr;
    auto imageProcessor = acl::dvpp::DvppManager::GetInstance().GetVideoProcessor();
    aclError ret = imageProcessor->CreateNotifyAndStremForVencChannel(channelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
}

TEST_F(VencTest, LaunchReleaseFrameTaskTest)
{
    aclvencChannelDesc vencChannelDesc;
    vencChannelDesc.dataBuffer.data = reinterpret_cast<void *>(0x1);
    vencChannelDesc.outputMemMode = 10;

    auto imageProcessor = acl::dvpp::DvppManager::GetInstance().GetVideoProcessor();
    aclError ret = imageProcessor->LaunchReleaseFrameTask(&vencChannelDesc);
    EXPECT_EQ(ret, ACL_SUCCESS);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCpuKernelLaunch(_, _, _, _, _, _, _))
        .WillOnce(Return(ACL_ERROR_RT_PARAM_INVALID));
    ret = imageProcessor->LaunchReleaseFrameTask(&vencChannelDesc);
    EXPECT_NE(ret, ACL_SUCCESS);
}

TEST_F(VencTest, GetVencFrameCallbackTest)
{
    void *callbackData = (void *)malloc(5);
    auto imageProcessor = acl::dvpp::DvppManager::GetInstance().GetVideoProcessor();
    imageProcessor->GetVencFrameCallback(callbackData);
    free(callbackData);
}

TEST_F(VencTest, aclvencGetChannelDescIPPropTest)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc vencChannelDesc;
    vencChannelDesc.dataBuffer.data = reinterpret_cast<void *>(0x1);
    bool isSupport;
    auto imageProcessor = acl::dvpp::DvppManager::GetInstance().GetVideoProcessor();
    acl::dvpp::DvppManager &dvppManager = acl::dvpp::DvppManager::GetInstance();

    uint32_t ipProp = 1;
    imageProcessor->aclvencSetChannelDescIPProp(&vencChannelDesc, ipProp);
    imageProcessor->aclvencGetChannelDescIPProp(&vencChannelDesc, isSupport);
}

TEST_F(VencTest, aclvencV200_LaunchVencWaitTaskTest)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *channelDesc = aclvencCreateChannelDesc();
    channelDesc->vencWaitTaskType = NOTIFY_TASK;
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyWait(_, _))
        .WillOnce(Return((1)));
    v200.LaunchVencWaitTask(channelDesc);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyWait(_, _))
        .WillOnce(Return((0)))
        .WillRepeatedly(Return(1));
    v200.LaunchVencWaitTask(channelDesc);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    channelDesc->vencWaitTaskType = EVENT_TASK;
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamWaitEvent(_, _))
        .WillOnce(Return((1)));
    v200.LaunchVencWaitTask(channelDesc);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamWaitEvent(_, _))
        .WillOnce(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventReset(_, _))
        .WillRepeatedly(Return(1));
    v200.LaunchVencWaitTask(channelDesc);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamWaitEvent(_, _))
        .WillOnce(Return(0))
        .WillRepeatedly(Return(1));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventReset(_, _))
        .WillRepeatedly(Return(0));
    v200.LaunchVencWaitTask(channelDesc);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamWaitEvent(_, _))
        .WillOnce(Return(0))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventReset(_, _))
        .WillOnce(Return(0))
        .WillRepeatedly(Return(1));
    v200.LaunchVencWaitTask(channelDesc);
    aclvencDestroyChannelDesc(channelDesc);
}

TEST_F(VencTest, aclvencV200_aclvencSendNomalFrameTest)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *channelDesc = aclvencCreateChannelDesc();
    acldvppPicDesc *input = acldvppCreatePicDesc();
    void *reserve = nullptr;
    aclvencFrameConfig *config = aclvencCreateFrameConfig();;
    void *userdata = nullptr;
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpy(_, _, _, _, _))
        .WillOnce(Return(1));
    v200.aclvencSendNomalFrame(channelDesc, input, reserve, config, userdata);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpy(_, _, _, _, _))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMalloc(_, _, _))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCpuKernelLaunch(_, _, _, _, _, _, _))
        .WillOnce(Return(ACL_ERROR_RT_PARAM_INVALID));
    v200.aclvencSendNomalFrame(channelDesc, input, reserve, config, userdata);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpy(_, _, _, _, _))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMalloc(_, _, _))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtNotifyWait(_, _))
        .WillOnce(Return(ACL_ERROR_RT_PARAM_INVALID));
    v200.aclvencSendNomalFrame(channelDesc, input, reserve, config, userdata);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMemcpy(_, _, _, _, _))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtMalloc(_, _, _))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCallbackLaunch(_, _, _, _))
        .WillOnce(Return(ACL_ERROR_RT_PARAM_INVALID));
    v200.aclvencSendNomalFrame(channelDesc, input, reserve, config, userdata);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));
    aclvencDestroyChannelDesc(channelDesc);
    acldvppDestroyPicDesc(input);
    aclvencDestroyFrameConfig(config);
}

TEST_F(VencTest, aclvencV200_SendEosForVencTest)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *channelDesc = aclvencCreateChannelDesc();
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamSynchronize(_))
        .WillOnce(Return((1)));
    v200.SendEosForVenc(channelDesc);
    aclvencDestroyChannelDesc(channelDesc);
}

TEST_F(VencTest, aclvencV200_aclvdecSetChannelDescOutPicFormatTest)
{
    aclvdecChannelDesc *channelDesc;
    acldvppPixelFormat outPicFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);
    v200.aclvdecSetChannelDescOutPicFormat(channelDesc, outPicFormat);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    channelDesc = nullptr;
    v200.aclvdecSetChannelDescOutPicFormat(channelDesc, outPicFormat);
}

TEST_F(VencTest, aclvencV200_aclvdecSetChannelDescChannelIdTest)
{
    aclvdecChannelDesc *channelDesc = nullptr;
    uint32_t channelId = 1;
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);
    v200.aclvdecSetChannelDescChannelId(channelDesc, channelId);
}

TEST_F(VencTest, aclvencV200_LaunchTaskForGetStreamTest)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvdecChannelDesc *channelDesc = aclvdecCreateChannelDesc();
    channelDesc->vdecWaitTaskType = EVENT_TASK;
    aicpu::dvpp::VdecCallbackInfoPtr callbackInfoPtr;
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamWaitEvent(_, _))
        .WillOnce(Return((1)));
    v200.LaunchTaskForGetStream(channelDesc, callbackInfoPtr);
    Mock::VerifyAndClear((void *)(&MockFunctionTest::aclStubInstance()));

    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtStreamWaitEvent(_, _))
        .WillOnce(Return((0)));
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtEventReset(_, _))
        .WillRepeatedly(Return((1)));
    v200.LaunchTaskForGetStream(channelDesc, callbackInfoPtr);
    aclvdecDestroyChannelDesc(channelDesc);
}

TEST_F(VencTest, aclvencV200_LaunchReleaseFrameTaskTest)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    aclvencChannelDesc *channelDesc = aclvencCreateChannelDesc();
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), rtCpuKernelLaunch(_, _, _, _, _, _, _))
        .WillOnce(Return(1));
    v200.LaunchReleaseFrameTask(channelDesc);
    aclvencDestroyChannelDesc(channelDesc);
}

TEST_F(VencTest, aclvencV200_GetVencFrameCallbackV200Test)
{
    void *callbackData;
    acl::dvpp::VideoProcessorV200 v200(ACL_HOST);
    v200.GetVencFrameCallbackV200(callbackData);
}

static rtError_t rtFreeStub(void *devPtr)
{
    // free(devPtr);
    return RT_ERROR_NONE;
}

TEST_F(VencTest, VideoProcessorV200_0001)
{
    EXPECT_CALL(MockFunctionTest::aclStubInstance(), mmAlignMalloc(_, _))
        .WillRepeatedly(Invoke(mmAlignMallocStubDvpp));
    acl::dvpp::VideoProcessorV200 videoProcessor(ACL_HOST);
    aclvencChannelDesc channelDesc;
    channelDesc.bufAddr = 0;
    channelDesc.bufSize = 10;
    EXPECT_EQ(videoProcessor.aclvencMallocOutMemory(&channelDesc), ACL_ERROR_RT_FAILURE);
}