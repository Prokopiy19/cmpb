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

json prepare_branch(const std::string& branch_name,
                    const std::string& arch)
{
    auto response = get_branch(branch_name, arch);
    auto object = json::parse(response);
    auto packages = object["packages"];
    return packages;
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
    
    for (const auto& arch : archs) {
        std::cout << arch << std::endl;
        auto packages0 = prepare_branch(branch_name0, arch);
        auto packages1 = prepare_branch(branch_name1, arch);
        sort_by_name(packages0);
        sort_by_name(packages1);
        auto ans = compare_branches(packages0, packages1);
        for (const auto& result : ans) {
            std::cout << result << std::endl;
        }
    }
    return 0;
}