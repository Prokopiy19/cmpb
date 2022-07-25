#include <iostream>
#include "json.hpp"
#include "utils.h"
using json = nlohmann::json;

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
    auto groups0 = prepare_branch(branch_name0);
    auto groups1 = prepare_branch(branch_name1);
    std::cout << branch_name0 << std::endl;
    for (const auto& group : groups0) {
        std::cout << group.first << ' '  << group.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << branch_name1 << std::endl;
    for (const auto& group : groups1) {
        std::cout << group.first << ' '  << group.second << std::endl;
    }
    return 0;
}