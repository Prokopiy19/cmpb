#include <iostream>
#include "utils.h"

int main()
{
    auto ans = get_branch("sisyphus");
    std::cout << ans.substr(0, 500);
}