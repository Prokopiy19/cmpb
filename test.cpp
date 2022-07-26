#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include "json.hpp"
#include "utils.h"
using json = nlohmann::json;

void prepare_archs(const std::string& branch_name,
                   std::vector<std::string>& archs)
{
    auto resp = get_archs(branch_name);
    json json_archs = json::parse(resp)["archs"];
    std::transform(json_archs.cbegin(), json_archs.cend(), std::back_inserter(archs), 
        [](const json& object) { return object["arch"]; });
}

std::map<std::string, json> prepare_branch(const std::string& branch_name)
{
    auto response = get_branch(branch_name);
    auto value = json::parse(response);
    auto packages = value["packages"];
    auto groups = group_by_arch(packages);
    return groups;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "not enough arguments" << std::endl;
        return 0;
    }
    const std::string branch_name0 = argv[1];
    const std::string branch_name1 = argv[2];
    
    std::vector<std::string> archs;
    prepare_archs(branch_name0, archs);
    prepare_archs(branch_name1, archs);
    std::sort(archs.begin(), archs.end());
    archs.erase(std::unique(archs.begin(), archs.end()), archs.end());
    for (const auto& s : archs) {
        std::cout << s << ' ';
    }
    std::cout << std::endl;
    auto groups0 = prepare_branch(branch_name0);
    auto groups1 = prepare_branch(branch_name1);
    sort_by_name(groups0);
    sort_by_name(groups1);
    auto ans = compare_branches(groups0, groups1);
    for (const auto& [arch, result] : ans) {
        std::cout << arch << ' ' << result << std::endl;
    }
    return 0;
}