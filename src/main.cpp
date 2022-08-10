#define NDEBUG
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "cmpb/cmpb.hpp"
using json = nlohmann::json;

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "not enough arguments" << std::endl;
        return -2;
    }
    const std::string branch_name0 = argv[1];
    const std::string branch_name1 = argv[2];
    
    std::vector<std::string> archs;
    bool success = get_archs(branch_name0, archs);
        success &= get_archs(branch_name1, archs);
    if (!success) {
        return -1;
    }
    std::sort(archs.begin(), archs.end());
    archs.erase(std::unique(archs.begin(), archs.end()), archs.end());
    
    for (const auto& arch : archs) {
        std::cout << arch << std::endl;
        json packages0 = get_branch(branch_name0, arch);
        json packages1 = get_branch(branch_name1, arch);
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
