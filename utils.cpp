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

std::string get_branch(const std::string& branch_name)
{
    const std::string get_url = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    return curl_get(get_url + branch_name);
}

std::map<std::string, json> group_by_arch(const json& packages)
{
    std::map<std::string, json> ret;
    for (const auto& entry : packages)
        ret[std::string(entry["arch"])].emplace_back(entry);
    return ret;
}

void sort_by_name(std::map<std::string, json> groups)
{
    for (auto& [arch, group]: groups) {
        sort(group.begin(), group.end(),
            [](const auto& lhs, const auto& rhs)
            { return lhs["name"] < rhs["name"]; });
    }
}

std::map<std::string, json> compare_branches(const std::map<std::string, json>& groups0,
                                             const std::map<std::string, json>& groups1)
{
    std::map<std::string, json> ret;
    for (const auto& [arch, group0] : groups0) {
        auto it = groups1.find(arch);
        if (it == groups1.cend())
            continue;
        const json& group1 = it->second;
        auto only_in_first = json::array();
        for (const auto& package0 : group0) {
            std::string name = package0["name"];
            auto l = std::lower_bound(group1.cbegin(), group1.cend(), name,
                [](const auto& lhs, const auto& rhs)
                { return lhs["name"] < rhs; });
            if (l != group1.cend() && (*l)["name"] == name)
                continue;
            else
                only_in_first.emplace_back(package0);
        }
        ret[arch] = only_in_first;
    }
    return ret;
}