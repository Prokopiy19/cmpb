#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include "json.hpp"
using nlohmann::json;

std::string get_branch(const std::string& branch_name);
std::map<std::string, json> group_by_arch(const json& packages);


#endif