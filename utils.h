#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include "json.hpp"
using nlohmann::json;

std::string get_archs(const std::string& branch_name);

std::string get_branch(const std::string& branch_name, const std::string& arch);

void sort_by_name(json& packages);

json compare_branches(const json& branch0, const json& branch1);

#endif