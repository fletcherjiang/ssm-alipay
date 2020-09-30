/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>
#include <string>
#include <securec.h>
#include "framework/common/ge_format_util.h"
#include "framework/executor/ge_executor.h"
#include "framework/generator/ge_generator.h"
#include "framework/common/util.h"
#include "common/helper/om_file_helper.h"
#include "graph/utils/graph_utils.h"
#include "graph/utils/attr_utils.h"
#include "graph/utils/tensor_utils.h"
#include "graph/utils/op_desc_utils.h"
#include "graph/ge_tensor.h"
#include "graph/utils/type_utils.h"
#include "graph/tensor.h"
#include "graph/model.h"
#include "graph/ge_attr_value.h"
#include "graph/ge_error_codes.h"
#include "graph/types.h"
#include "graph/operator.h"
#include "ge/ge_api.h"
#include "common/ge_types.h"
#include "graph/debug/ge_attr_define.h"
#include "graph/opsproto_manager.h"
#include "graph/operator_factory.h"
#include "graph/ge_local_context.h"
#include "toolchain/prof_callback.h"

namespace {
    std::string STAGES_STR = "[TEST][TEST]";
}

namespace ge {

const std::string ATTR_NAME_STORAGE_FORMAT = "storage_format";
const std::string ATTR_NAME_STORAGE_SHAPE = "storage_shape";
const std::string ATTR_NAME_UNREGST_OPPATH = "_unregst_oppath";
const std::string ATTR_NAME_UNREGST_ATTRLIST = "_unregst _attrlist";
const std::string ATTR_NAME_DYNAMIC_INPUT_START = "_dynamic_input_index_start";
const std::string ATTR_NAME_DYNAMIC_INPUT_END = "_dynamic_input_index_end";
const std::string ATTR_NAME_WEIGHTS = "value";
const std::string CONST_ATTR_NAME_INPUT = "is_const";
const std::string ATTR_NAME_FUZZ_BUILD_RES_ATTRS = "_fuzz_build_res";
const std::string ATTR_NAME_PLACEMENT = "_mem_type";
const std::string ATTR_NAME_FUZZ_INPUTS_SUPPORTED_ATTRS = "_inputs_support_info";
const std::string ATTR_NAME_FUZZ_OUTPUTS_SUPPORTED_ATTRS = "_outputs_support_info";
const std::string ATTR_NAME_BUILD_MODE = "_build_mode";
const std::string ATTR_NAME_VALUE = "_value";
const std::string ATTR_NAME_VALUE_RANGE = "_value_range";

namespace {
bool g_geAttrValueBool;
std::string g_geAttrValueString;
float g_geAttrValueFloat;
DataType g_geAttrValueDataType;
int64_t g_geAttrValueInt;
thread_local GEThreadLocalContext threadContext;
GeAttrValue g_geAttrValue;

std::vector<bool> g_geAttrValueListBool;
std::vector<std::string> g_geAttrValueListString;
std::vector<float> g_geAttrValueListFloat;
std::vector<int64_t> g_geAttrValueListInt;
std::vector<std::vector<int64_t>> g_geAttrValueListListInt;
std::vector<std::vector<float>> g_geAttrValueListListFloat;

ge::GeAttrValue::ValueType g_geAttrValueType = ge::GeAttrValue::VT_FLOAT;

std::map<string, GeAttrValue> g_geAttrMap;
}

    TensorDesc::TensorDesc(void)
    {
    }

    TensorDesc::TensorDesc(TensorDesc const& desc)
    {
    }

    GeExecutor::GeExecutor(void)
    {
    }

    Status GeExecutor::Initialize()
    {
        return SUCCESS;
    }

    Status GeExecutor::Finalize()
    {
        return SUCCESS;
    }

    Status GeExecutor::CommandHandle(const ge::Command &command)
    {
        return SUCCESS;
    }

    Status GeExecutor::GetDeviceIdByModelId(uint32_t model_id, uint32_t &device_id)
    {
        return SUCCESS;
    }

    Status GeExecutor::SetDump(const DumpConfig &dumpConfig)
    {
        return SUCCESS;
    }

    Status GeExecutor::ReleaseSingleOpResource(void *stream)
    {
        return SUCCESS;
    }

    Status GeExecutor::GetModelDescInfo(uint32_t modelId, std::vector<TensorDesc>& inputDesc,
                                            std::vector<TensorDesc>& outputDesc, bool new_model_desc)
    {
        return SUCCESS;
    }

    Status GeExecutor::SetDynamicAippData(uint32_t model_id, void *dynamic_input_addr, uint64_t length,
                                            const std::vector<kAippDynamicBatchPara> &aippBatchPara,
                                            const kAippDynamicPara &aippParms)
    {
        return SUCCESS;
    }

    Status GeExecutor::GetAIPPInfo(uint32_t model_id, uint32_t index, AippConfigInfo &aipp_params)
    {
        aipp_params.input_format = 1;
        return SUCCESS;
    }

    Status GeExecutor::GetAippType(uint32_t model_id, uint32_t index, ge::InputAippType &type, size_t &aippindex)
    {
        type = ge::DATA_WITH_DYNAMIC_AIPP;
        aippindex = 3;
        return SUCCESS;
    }

    Status GeExecutor::GetBatchInfoSize(uint32_t model_id, size_t &shape_count)
    {
        shape_count = 2;
        return SUCCESS;
    }

    Status GeExecutor::GetOrigInputInfo(uint32_t model_id, uint32_t index, OriginInputInfo &origOutputInfo)
    {
        origOutputInfo.format = static_cast<Format>(1);
        origOutputInfo.data_type = static_cast<DataType>(4);
        origOutputInfo.dim_num = 4;
        return SUCCESS;
    }

    Status GeExecutor::GetAllAippInputOutputDims(uint32_t model_id, uint32_t index,
                                                 std::vector<InputOutputDims> &input_dims,
                                                 std::vector<InputOutputDims> &output_dims)
    {
        InputOutputDims inputDims1;
        inputDims1.dim_num = 4;
        inputDims1.dims.push_back(1);
        inputDims1.dims.push_back(224);
        inputDims1.dims.push_back(224);
        inputDims1.dims.push_back(3);
        InputOutputDims inputDims2;
        input_dims.push_back(inputDims1);
        input_dims.push_back(inputDims2);
        output_dims.push_back(inputDims1);
        output_dims.push_back(inputDims2);
        return SUCCESS;
    }

    Status GeExecutor::SetDynamicBatchSize(uint32_t model_id, void *dynamic_input_addr, uint64_t length, uint64_t batch_size)
    {
        return SUCCESS;
    }

    Status GeExecutor::SetDynamicImageSize(uint32_t model_id, void *dynamic_input_addr, uint64_t length, uint64_t image_height, uint64_t image_width)
    {
        return SUCCESS;
    }

    Status GeExecutor::SetDynamicDims(uint32_t model_id, void *dynamic_input_addr, uint64_t length,
                                      const vector<uint64_t> &dynamic_dims)
    {
      return SUCCESS;
    }

    Status GeExecutor::GetCurDynamicDims(uint32_t model_id, const vector<uint64_t> &dynamic_dims,
                                         vector<uint64_t> &cur_dynamic_dims)
    {
      return SUCCESS;
    }

    int64_t TensorDesc::GetSize() const
    {
        return 1;
    }

    std::string TensorDesc::GetName() const
    {
        return "resnet50";
    }

    graphStatus TensorDesc::GetName(AscendString &name)
    {
        return 0;
    }

    AscendString::AscendString(char const *name) { }

    Format TensorDesc::GetFormat() const
    {
        Format format = FORMAT_NCHW;
        return format;
    }

    DataType TensorDesc::GetDataType() const
    {
        DataType dt = DT_FLOAT;
        return dt;
    }

    Shape TensorDesc::GetShape() const
    {
        std::vector<int64_t> vec;
        Shape shape(vec);
        return shape;
    }

    graphStatus TensorDesc::GetShapeRange(std::vector<std::pair<int64_t,int64_t>> &range) const
    {
        return 0;
    }

    const char* AscendString::GetString() const
    {
        return "resnet50";
    }

    std::vector<int64_t> Shape::GetDims() const
    {
        std::vector<int64_t> vec;
        vec.push_back(1);
        return vec;
    }

    Status GeExecutor::GetDynamicBatchInfo(uint32_t model_id, std::vector<std::vector<int64_t>> &batch_info,
                                           int32_t &dynamic_type)
    {
        batch_info.push_back({1});
        return ge::SUCCESS;
    }

    Status GeExecutor::GetCombinedDynamicDims(uint32_t model_id, vector<vector<int64_t>> &batch_info)
    {
      return ge::SUCCESS;
    }
    Status GeExecutor::GetUserDesignateShapeOrder(uint32_t model_id, vector<string> &user_designate_shape_order)
    {
        return ge::SUCCESS;
    }
    Status GeExecutor::GetCurShape(const uint32_t model_id, std::vector<int64_t> &batch_info, int32_t &dynamic_type)
    {
        batch_info.push_back(1);
        return ge::SUCCESS;
    }

    Status GeExecutor::GetModelAttr(uint32_t model_id,std::vector<std::string> &dynamic_output_shape_info)
    {
        dynamic_output_shape_info.push_back({"0:0:1,3,224,224"});
        return ge::SUCCESS;
    }

    Status GeExecutor::LoadDataFromFile(std::string const &path, ModelData &modelData)
    {
        return SUCCESS;
    }

    Status GeExecutor::LoadModelFromData(uint32_t &model_id, const ModelData &modelData,
                                   void *dev_ptr, size_t memsize, void *weight_ptr, size_t weightsize)
    {
        return SUCCESS;
    }

    Status GeExecutor::LoadModelWithQ(uint32_t &model_id, const ge::ModelData &ge_model_data,
                   const std::vector<uint32_t> &input_queue_ids, const std::vector<uint32_t> &output_queue_ids)
    {
        return SUCCESS;
    }

    Status GeExecutor::UnloadModel(uint32_t modelId)
    {
        return SUCCESS;
    }

    Status GeExecutor::ExecModel(uint32_t model_id, void *stream, const ge::RunModelData &run_input_data,
                                const std::vector<ge::GeTensorDesc> &input_desc, ge::RunModelData &run_output_data,
                                std::vector<ge::GeTensorDesc> &output_desc, bool async_mode)
    {
        ge::GeTensorDesc geDescTmp;
        output_desc.push_back(geDescTmp);
        return SUCCESS;
    }

    Status GeExecutor::GetMemAndWeightSize(const void *model_data, size_t model_size, size_t &mem_size, size_t &weight_size)
    {
        return SUCCESS;
    }

    Status GeExecutor::GetMemAndWeightSize(const std::string &path, size_t &mem_size, size_t &weight_size)
    {
        return SUCCESS;
    }

    Status GeExecutor::ExecuteAsync(SingleOp *executor,
                                    const std::vector<DataBuffer> &inputs,
                                    std::vector<DataBuffer> &outputs)
    {
        return SUCCESS;
    }

    Status GeExecutor::ExecuteAsync(DynamicSingleOp *executor,
                        const std::vector<GeTensorDesc> &input_desc,
                        const std::vector<DataBuffer> &inputs,
                        std::vector<GeTensorDesc> &output_desc,
                        std::vector<DataBuffer> &outputs)
    {
        return SUCCESS;
    }

    Status GeExecutor::LoadSingleOpV2(const std::string &modelName,
                                      const ModelData &modelData,
                                      void *stream,
                                      SingleOp **single_op,
                                      const uint64_t model_id)
    {
        return SUCCESS;
    }

    Status GeExecutor::LoadDynamicSingleOpV2(const std::string &model_name,
                               const ge::ModelData &modelData,
                               void *stream,
                               DynamicSingleOp **single_op,
                               const uint64_t model_id)
    {
        return SUCCESS;
    }

    Status GeExecutor::GetOpDescInfo(uint32_t device_id, uint32_t stream_id,
        uint32_t task_id, OpDescInfo &op_desc_info)
    {
        op_desc_info.op_name = "cast";
        op_desc_info.task_id = task_id;
        op_desc_info.stream_id = stream_id;
        op_desc_info.input_format.push_back(FORMAT_NCHW);
        op_desc_info.output_format.push_back(FORMAT_NCHW);
        op_desc_info.input_data_type.push_back(DT_FLOAT);
        op_desc_info.output_data_type.push_back(DT_FLOAT);
        op_desc_info.input_shape.push_back({1, 1});
        op_desc_info.output_shape.push_back({1, 1});
        int a = 0;
        void *p = (void *)&a;
        op_desc_info.input_addrs.push_back(p);
        op_desc_info.output_addrs.push_back(p);
        return SUCCESS;
    }

    Model::Model(void)
    {
    }

    ProtoAttrMapHelper Model::MutableAttrMap()
    {
    }

    ConstProtoAttrMapHelper Model::GetAttrMap() const
    {
    }

    graphStatus Model::Load(unsigned char const*, unsigned long, Model&)
    {
        return 0;
    }

    GeAttrValue NamedAttrs::GetItem(const string &key) const
    {
        return g_geAttrValue;
    }

    NamedAttrs::NamedAttrs()
    {
    }
    ProtoAttrMapHelper NamedAttrs::MutableAttrMap()
    {
    }

    ConstProtoAttrMapHelper NamedAttrs::GetAttrMap() const
    {
    }

    bool AttrUtils::GetListTensor(AttrUtils::ConstAttrHolderAdapter&&  obj, const string& name, vector<ConstGeTensorPtr>& value)
    {
        return true;
    }

    bool AttrUtils::GetStr(AttrUtils::ConstAttrHolderAdapter&&  obj, const string& name, string& value)
    {
        return true;
    }

    bool AttrUtils::SetStr(AttrHolderAdapter &&obj, const string &name, const string &value)
    {
        return true;
    }

    bool  AttrUtils::SetListInt(AttrHolderAdapter &&obj, const string &name, const vector<int32_t> &value)
    {
        return true;
    }

    bool AttrUtils::SetBool(AttrHolderAdapter &&obj, const string &name, const bool &value)
    {
        return true;
    }

    bool AttrUtils::GetBool(ConstAttrHolderAdapter &&obj, const string &name, bool &value)
    {
        return true;
    }

    bool AttrUtils::HasAttr(ConstAttrHolderAdapter &&obj, const string &name)
    {
        return true;
    }

    bool AttrUtils::SetTensor(AttrHolderAdapter &&obj, const string &name, const ConstGeTensorPtr &value)
    {
        return true;
    }

    bool AttrUtils::GetTensor(ConstAttrHolderAdapter &&obj, const string &name, ConstGeTensorPtr &value)
    {
        return true;
    }

    bool AttrUtils::GetListNamedAttrs(ge::AttrUtils::ConstAttrHolderAdapter&&, std::string const&, vector<GeAttrValue::NAMED_ATTRS> &)
    {
        return true;
    }

    std::uint8_t *TensorData::GetData()
    {
        return nullptr;
    }

    const std::uint8_t *TensorData::GetData() const
    {
        return nullptr;
    }

    std::size_t TensorData::GetSize() const
    {
        return 0;
    }

    Buffer::Buffer()
    {
    }

    std::uint8_t *Buffer::GetData()
    {
        return nullptr;
    }

    const std::uint8_t *Buffer::GetData() const
    {
        return nullptr;
    }

    std::size_t Buffer::GetSize() const
    {
        return 0;
    }

    graphStatus AttrHolder::DelAttr(std::string const&)
    {
        return 0;
    }

    std::vector<int64_t> GeShape::GetDims() const
    {
        vector<int64_t> vec;
        return vec;
    }

    size_t GeShape::GetDimNum() const
    {
        return 0;
    }

    GeShape &GeShape::operator=(const GeShape &other)
    {

    }

    GeShape &GeShape::operator=(GeShape &&other)
    {

    }

    const std::map<string, GeAttrValue> AttrHolder::GetAllAttrs() const
    {
        GeAttrValue attr;
        std::string name = "ATTR_MODEL_test";
        std::map<string, GeAttrValue> m;
        m.insert(std::make_pair(name, attr));
        return m;
    }

    GeTensorDesc::GeTensorDesc()
    {
    }

    GeTensorDesc::GeTensorDesc(GeTensorDesc&&)
    {
    }

    GeTensorDesc::GeTensorDesc(GeTensorDesc const&)
    {
    }

    GeTensorDesc::GeTensorDesc(GeShape, Format, DataType)
    {
    }

    ProtoAttrMapHelper GeTensorDesc::MutableAttrMap()
    {
    }

    ConstProtoAttrMapHelper GeTensorDesc::GetAttrMap() const
    {
    }

    void GeTensorDesc::SetOriginFormat(Format originFormat)
    {
    }

    void GeTensorDesc::SetFormat(Format format)
    {
        return;
    }

    Format GeTensorDesc::GetFormat() const
    {
        return FORMAT_NCHW;
    }

    Format GeTensorDesc::GetOriginFormat() const
    {
        return FORMAT_NCHW;
    }

    GeShape& GeTensorDesc::MutableShape()
    {
        static GeShape shape;
        return shape;
    }

    DataType GeTensorDesc::GetDataType() const
    {
        return DT_FLOAT;
    }

    void GeTensorDesc::SetShape(GeShape shape)
    {
        return;
    }

    GeShape GeTensorDesc::GetShape() const
    {
        GeShape shape;
        return shape;
    }

    GeShape GeTensorDesc::GetOriginShape() const
    {
        GeShape shape;
        return shape;
    }

    void GeTensorDesc::SetOriginShape(const GeShape &originShape)
    {
    }

    graphStatus GeTensorDesc::SetShapeRange(const std::vector<std::pair<int64_t,int64_t>> &range)
    {
        return GRAPH_SUCCESS;
    }

    void GeTensorDesc::SetDataType(ge::DataType dt)
    {
        return;
    }

    void GeTensorDesc::SetOriginDataType(DataType origin_data_type)
    {
        return;
    }

    graphStatus GeTensorDesc::GetShapeRange(std::vector<std::pair<int64_t,int64_t>> &range) const
    {
        range.push_back(std::make_pair(1, 16));
        range.push_back(std::make_pair(1, 16));
        range.push_back(std::make_pair(1, 16));
        range.push_back(std::make_pair(1, 16));
        return GRAPH_SUCCESS;
    }

    GeTensor::GeTensor()
    {
    }

    GeTensor::GeTensor(GeTensorDesc const&)
    {
    }

    GeTensor::GeTensor(GeTensor const&)
    {
    }

    GeTensor::GeTensor(const GeTensorDesc &tensorDesc, const uint8_t *data, size_t size)
    {
    }

    GeTensorDesc GeTensor::GetTensorDesc() const
    {
        GeTensorDesc tenosrDesc;
        return tenosrDesc;
    }


    GeAttrValue::GeAttrValue()
    {
    }

    graphStatus GeAttrValue::SetValue(bool const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_BOOL;
        g_geAttrValueBool = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(long const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_INT;
        g_geAttrValueInt = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(float const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_FLOAT;
        g_geAttrValueFloat = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(DataType const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_DATA_TYPE;
        g_geAttrValueDataType = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(std::string const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_STRING;
        g_geAttrValueString = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(std::vector<bool> const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_LIST_BOOL;
        g_geAttrValueListBool = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(std::vector<long> const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_LIST_INT;
        g_geAttrValueListInt = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(std::vector<float> const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_LIST_FLOAT;
        g_geAttrValueListFloat = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(std::vector<std::string> const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_LIST_STRING;
        g_geAttrValueListString = value;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::SetValue(std::vector<std::vector<int64_t>> const& value)
    {
        g_geAttrValueType = GeAttrValue::VT_LIST_LIST_INT;
        g_geAttrValueListListInt = value;
        return GRAPH_SUCCESS;
    }


    GeAttrValue::ValueType GeAttrValue::GetValueType() const
    {
        return g_geAttrValueType;
    }

    graphStatus GeAttrValue::GetValue(bool& value) const
    {
        value = g_geAttrValueBool;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(long& value) const
    {
        value = g_geAttrValueInt;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(float& value) const
    {
        value = g_geAttrValueFloat;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(DataType& value) const
    {
        value = g_geAttrValueDataType;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(string& value) const
    {
        value = g_geAttrValueString;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(std::vector<std::string>& value) const
    {
        value = g_geAttrValueListString;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(std::vector<bool>& value) const
    {
        value = g_geAttrValueListBool;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(std::vector<float>& value) const
    {
        value = g_geAttrValueListFloat;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(std::vector<long>& value) const
    {
        value = g_geAttrValueListInt;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(std::vector<std::vector<int64_t>>& value) const
    {
        value = g_geAttrValueListListInt;
        return GRAPH_SUCCESS;
    }

    graphStatus GeAttrValue::GetValue(std::vector<std::vector<float>>& value) const
    {
        value = g_geAttrValueListListFloat;
        return GRAPH_SUCCESS;
    }

    graphStatus OpDesc::AddInputDesc(const string &name, const GeTensorDesc &input_desc)
    {
        return GRAPH_SUCCESS;
    }

    graphStatus OpDesc::AddOutputDesc(const string &name, const GeTensorDesc &output_desc)
    {
        return GRAPH_SUCCESS;
    }

    OpDesc::OpDesc()
    {
    }

    OpDesc::~OpDesc()
    {
    }

    std::string OpDesc::GetName() const
    {
        return "OpName";
    }

    OpDesc::OpDesc(std::string const&, std::string const&)
    {
    }

    std::string OpDesc::GetInputNameByIndex(uint32_t index) const
    {
        return "";
    }

    ProtoAttrMapHelper OpDesc::MutableAttrMap()
    {
    }

    ConstProtoAttrMapHelper OpDesc::GetAttrMap() const
    {
    }


    void TensorUtils::SetRealDimCnt(GeTensorDesc& tensorDesc, uint32_t cnt)
    {
    }

    void TensorUtils::SetInputTensor(GeTensorDesc& tensorDesc, bool flag)
    {
    }

    void TensorUtils::SetOutputTensor(GeTensorDesc& tensorDesc, bool flag)
    {
    }


    graphStatus OpDesc::AddInputDesc(const GeTensorDesc& input_desc)
    {
        return GRAPH_SUCCESS;
    }

    graphStatus OpDesc::AddOutputDesc(const GeTensorDesc& output_desc)
    {
        return GRAPH_SUCCESS;
    }

    GeShape::GeShape()
    {
    }

    GeShape::GeShape(GeShape const&)
    {
    }

    graphStatus AttrHolder::SetAttr(const string& name, const GeAttrValue& value)
    {
        return GRAPH_SUCCESS;
    }

    GeShape::GeShape(std::vector<long>)
    {
    }

    Status GEInitialize(const std::map<std::string, std::string>& options)
    {
        return SUCCESS;
    }

    Status GEFinalize()
    {
        return SUCCESS;
    }

    Status GeGenerator::Initialize(const std::map<std::string, std::string> &options)
    {
        return SUCCESS;
    }

    Status GeGenerator::Initialize(const std::map<std::string, std::string> &options, OmgContext &omgContext)
    {
        return SUCCESS;
    }

    Status GeGenerator::BuildSingleOpModel(ge::OpDescPtr &op_desc, const std::vector<GeTensor> &inputs,
                                           const std::vector<GeTensor> &outputs, OpEngineType engine_type,
                                           int32_t compile_flag, ModelBufferData &model_buff)
    {
        return SUCCESS;
    }

    Status GeGenerator::Finalize()
    {
        return SUCCESS;
    }

    Shape::Shape(const std::vector<int64_t>& dims)
    {
    }

    TensorDesc::TensorDesc(Shape shape, Format format, DataType dt)
    {
    }

    Status GeFormatUtil::TransShape(const TensorDesc &src_desc,
                                    Format dst_format,
                                    std::vector<int64_t> &dst_shape)
    {
        return SUCCESS;
    }

    bool AttrUtils::GetInt(ge::AttrUtils::ConstAttrHolderAdapter&& obj, const std::string &name, int64_t &value)
    {
        return true;
    }

    bool AttrUtils::GetInt(ge::AttrUtils::ConstAttrHolderAdapter&& obj, const std::string &name, int32_t &value)
    {
        return true;
    }

    bool AttrUtils::SetInt(ge::AttrUtils::AttrHolderAdapter &&obj, const string &name, const int64_t &value)
    {
       return true;
    }

    bool AttrUtils::SetListInt(ge::AttrUtils::AttrHolderAdapter &&obj, const string &name, const vector<int64_t> &value)
    {
       return true;
    }

    bool AttrUtils::GetListInt(ge::AttrUtils::ConstAttrHolderAdapter &&obj, std::string const&, std::vector<int64_t> &value)
    {
        return true;
    }

} // namespace ge

namespace ge {
    Status OmFileLoadHelper::Init(unsigned char*, unsigned int)
    {
        return ge::SUCCESS;
    }

    Status OmFileLoadHelper::GetModelPartition(ModelPartitionType, ModelPartition&)
    {
        return ge::SUCCESS;
    }

    bool ReadBytesFromBinaryFile(char const*, char**, int&)
    {
        return true;
    }

    std::string RealPath(const char *path)
    {
        return "test";
    }

    TensorDesc Operator::GetOutputDesc(uint32_t index) const
    {
        std::vector<int64_t> vec;
        Shape shape(vec);
        Format format = FORMAT_NCHW;
        DataType dt = DT_FLOAT;
        TensorDesc tensorDesc(shape, format, dt);
        return tensorDesc;
    }

    Operator &Operator::SetAttr(const string &name, const Tensor &attr_value)
    {

    }

    Operator &Operator::SetAttr(const char *name, const Tensor &attr_value) { }

    void Operator::BreakConnect() const {
        return;
    }

    graphStatus Operator::InferShapeAndType()
    {
        return 0;
    }

    Operator &Operator::SetInput(const string &dst_name, const Operator &src_oprt, const string &name)
    {

    }

    Operator &Operator::SetInput(const char *dst_name, const Operator &src_oprt, const char *name){ }

    OpsProtoManager *OpsProtoManager::Instance() { }

    bool OpsProtoManager::Initialize(const std::map<std::string, std::string> &options)
    {
        return true;
    }

    Operator OperatorFactory::CreateOperator(const std::string &operator_name, const std::string &operator_type)
    {
        Operator op;
        return op;
    }

    Operator OperatorFactory::CreateOperator(const char *operator_name, const char *operator_type)
    {
        Operator op;
        return op;
    }

    graphStatus OperatorFactory::GetOpsTypeList(std::vector<std::string> &all_ops)
    {
        return 0;
    }

    graphStatus OperatorFactory::GetOpsTypeList(std::vector<ge::AscendString> &all_ops)
    {
        return 0;
    }

    OpDescPtr OpDescUtils::GetOpDescFromOperator(const Operator& oprt)
    {
        return nullptr;
    }

    Operator OpDescUtils::CreateOperatorFromOpDesc(OpDescPtr op_desc)
    {
        Operator op;
        return op;
    }

    Tensor::Tensor(const TensorDesc &tensorDesc, const uint8_t *data, size_t size)
    {

    }

    std::map<string, uint32_t> OpDesc::GetAllInputName() const
    {
        std::map<string, uint32_t> mapStub;
        return mapStub;
    }

    GEThreadLocalContext &GetThreadLocalContext() { return threadContext; }

    void GEThreadLocalContext::SetGlobalOption(map<string, string> options_map)
    {
        return;
    }

    bool TypeUtils::IsInternalFormat(ge::Format format) {
        if (format == FORMAT_NCHW) {
            return false;
        }
        return true;
    }

const uint32_t MODEL_FILE_MAGIC_NUM = 0x444F4D49;
const uint32_t MODEL_FILE_HEAD_LEN = 256;
const uint32_t MODEL_VERSION = 0x10000000;

}// namespace ge

ge::Status RegProfCtrlCallback(MsprofCtrlCallback func)
{
    return 0;
}

ge::Status RegProfReporterCallback(MsprofReporterCallback func)
{
    return 0;
}

ErrorManager &ErrorManager::GetInstance() {
  static ErrorManager instance;
  return instance;
}

void ErrorManager::SetStage(const std::string &firstStage, const std::string &secondStage)
{
    STAGES_STR = '[' + firstStage + "][" + secondStage + ']';
}

const string& ErrorManager::GetLogHeader()
{
    return STAGES_STR;
}

int ErrorManager::Init()
{
    return 0;
}

void ErrorManager::ATCReportErrMessage(std::string error_code, const std::vector<std::string> &key,
                                       const std::vector<std::string> &value)
{

}

std::string ErrorManager::GetErrorMessage()
{
    std::string message = "";
    return message;
}

int ErrorManager::ReportInterErrMessage(std::string error_code, const std::string &error_msg)
{
    return 0;
}

int error_message::FormatErrorMessage(char *str_dst, size_t dst_max, const char *format, ...)
{
    return 1;
}
