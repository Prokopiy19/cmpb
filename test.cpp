#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include "json.hpp"
#include "utils.h"
using json = nlohmann::json;
#define NDEBUG

bool prepare_archs(const std::string& branch_name,
                   std::vector<std::string>& archs)
{
    auto resp = get_archs(branch_name);
    if (resp == FAIL) {
        std::cout << "failed to get branch " << branch_name << std::endl;
        return false;
    }
    json json_archs = json::parse(resp)["archs"];
    std::transform(json_archs.cbegin(), json_archs.cend(), std::back_inserter(archs), 
        [](const json& object) { return object["arch"]; });
    return true;
}

json prepare_branch(const std::string& branch_name,
                    const std::string& arch)
{
    auto response = get_branch(branch_name, arch);
    if (response == FAIL) {
        return nullptr;
    }
    auto object = json::parse(response);
    auto packages = object["packages"];
    return packages;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "not enough arguments" << std::endl;
        return -2;
    }
    const std::string branch_name0 = argv[1];
    const std::string branch_name1 = argv[2];
    
    std::vector<std::string> archs;
    bool success = prepare_archs(branch_name0, archs);
        success &= prepare_archs(branch_name1, archs);
    if (!success) {
        return -1;
    }
    std::sort(archs.begin(), archs.end());
    archs.erase(std::unique(archs.begin(), archs.end()), archs.end());
    
    for (const auto& arch : archs) {
        std::cout << arch << std::endl;
        json packages0 = prepare_branch(branch_name0, arch);
        json packages1 = prepare_branch(branch_name1, arch);
        sort_by_name(packages0);
        sort_by_name(packages1);
        json result1 = compare_branches(packages0, packages1);
        json result2 = compare_branches(packages1, packages0);
        json result3 = compare_versions(packages0, packages1);

        json ans1 = {
            {"description", "все пакеты, которые есть в 1-й но нет во 2-й"},
            {"packages", result1},
            {"length", result1.size()}
        };
        json ans2 = {
            {"description", "все пакеты, которые есть в 2-й но их нет во 1-й"},
            {"packages", result2},
            {"length", result2.size()}

        };
        json ans3 = {
            {"description", "все пакеты, version которых больше в 1-й чем во 2-й"},
            {"packages", result3},
            {"length", result3.size()}
        };

        json ans = {
            {"arch", arch },
            {"tasks", {ans1, ans2, ans3}}
        };
        std::ofstream record("results_"+arch+".json");
        record << ans.dump(4);
    }
    return 0;
}