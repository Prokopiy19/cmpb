#include <iostream>
#include "json.hpp"
#include "utils.h"
using json = nlohmann::json;

int main(int argc, char **argv)
{
    auto response = get_branch("sisyphus");
    auto value = json::parse(response);
    auto packages = value["packages"];
    auto groups = group_by_arch(packages);
    for (const auto& group : groups) {
        std::cout << group.first << ' '  << group.second << std::endl;
    }
}