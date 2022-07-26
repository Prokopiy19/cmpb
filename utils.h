#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include "json.hpp"
using nlohmann::json;

std::string get_branch(const std::string& branch_name);

std::map<std::string, json> group_by_arch(const json& packages);

void sort_by_name(std::map<std::string, json> groups);

std::map<std::string, json> compare_branches(const std::map<std::string, json>& groups0,
                                             const std::map<std::string, json>& groups1);

#endif