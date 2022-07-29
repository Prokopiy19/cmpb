#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "json.hpp"
using nlohmann::json;

json compare_branches(const json& branch0, const json& branch1);

json compare_versions(const json& branch0, const json& branch1);

bool get_archs(const std::string& branch_name,
               std::vector<std::string>& archs);

json get_branch(const std::string& branch_name,
                const std::string& arch);

const std::string FAIL = "";

#endif