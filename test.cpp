#include <fstream>
#include "json.hpp"
#include "utils.h"
using json = nlohmann::json;

int main(int argc, char **argv)
{
    std::ofstream record("sisiphys.txt");
    auto response = get_branch("sisyphus");
    auto value = json::parse(response);
    auto packages = value["packages"];
    int times = 1;
    for (const auto& entry : packages) {
        record << times << ' '  << entry << std::endl;
        ++times;
    }
}