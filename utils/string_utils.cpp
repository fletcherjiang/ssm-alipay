/**
* @file string_utils.cpp
*
* Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "string_utils.h"

namespace acl {
namespace string_utils {
void Split(const std::string &str, char delim, std::vector<std::string> &elems)
{
    elems.clear();
    if (str.empty()) {
        elems.emplace_back("");
        return;
    }

    std::stringstream ss(str);
    std::string item;

    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

    auto strSize = str.size();
    if ((strSize > 0U) && (str[strSize - 1U] == delim)) {
        elems.emplace_back("");
    }
}

bool IsDigit(const std::string &str)
{
    if (str.empty()) {
        return false;
    }

    for (const char &c : str) {
        if (isdigit(static_cast<int32_t>(c)) == 0) {
            return false;
        }
    }
    return true;
}
} // namespace string_utils
} // namespace acl