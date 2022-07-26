#include "utils.h"

#include <curl/curl.h>
#include <algorithm>
#include <string>
#include "json.hpp"
using nlohmann::json;

size_t write_function(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

std::string curl_get(const std::string& url)
{
    auto curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        std::string response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl = NULL;
     
        return response_string;
    }
    return "";
}

std::string get_archs(const std::string& branch_name)
{
    const std::string get_url = "https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=";
    return curl_get(get_url + branch_name);
}

std::string get_branch(const std::string& branch_name, const std::string& arch)
{
    const std::string get_url = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    return curl_get(get_url + branch_name + "?arch=" + arch);
}

void sort_by_name(json& packages)
{
    std::sort(packages.begin(), packages.end(),
        [](const auto& lhs, const auto& rhs)
        { return lhs["name"] < rhs["name"]; });
}

// найти пакеты, которые есть в 1-й ветке, но не в 2-й
json compare_branches(const json& branch0, const json& branch1)
{
    auto ret = json::array();
    for (const auto& package0 : branch0) {
        std::string name = package0["name"];
        auto l = std::lower_bound(branch1.cbegin(), branch1.cend(), name,
            [](const auto& lhs, const auto& rhs)
            { return lhs["name"] < rhs; });
        if (l != branch1.cend() && (*l)["name"] == name)
            continue;
        else
            ret.emplace_back(package0);
    }
    return ret;
}