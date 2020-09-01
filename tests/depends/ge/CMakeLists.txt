# Copyright 2019-2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

add_definitions(-w)
project(STUB_GE)

file(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        "src/ge_stub.cpp"
)

include_directories(${BASE_DIR}/third_party/inc)
include_directories(${BASE_DIR}/third_party/inc/graphengine/inc)
include_directories(${BASE_DIR}/third_party/inc/graphengine/inc/framework)
include_directories(${BASE_DIR}/third_party/inc/metadef/inc)
include_directories(${BASE_DIR}/third_party/inc/metadef/inc/external)
include_directories(${BASE_DIR}/third_party/inc/graphengine/inc/external)
include_directories(${BASE_DIR}/third_party/inc/graphengine/inc)

add_library(ge_stub SHARED ${SRC_FILES})

target_link_libraries(ge_stub PRIVATE
        $<BUILD_INTERFACE:intf_pub>
        c_sec
        ascend_protobuf
)
