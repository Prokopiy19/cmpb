#include <iostream>
#include <iterator>
#include <map>
#include "json.hpp"
#include "utils.h"
using json = nlohmann::json;

json prepare_archs(const std::string& branch_name0,
                   const std::string& branch_name1)
{
    auto resp0 = get_archs(branch_name0);
    auto resp1 = get_archs(branch_name1);
    json archs0 = json::parse(resp0)["archs"];
    json archs1 = json::parse(resp1)["archs"];
    json archs = archs0;
    std::copy(archs1.cbegin(), archs1.cend(), std::back_inserter(archs));
    std::sort(archs.begin(), archs.end());
    archs.erase(std::unique(archs.begin(), archs.end()), archs.end());
    return archs;
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
    
    json archs = prepare_archs(branch_name0, branch_name1);
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