#include "cmpb.hpp"

#include <curl/curl.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
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

        constexpr int SUCCESS_CODE = 200;
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code != SUCCESS_CODE) {
            return FAIL;
        }

        curl_easy_cleanup(curl);
        curl = NULL;
     
        return response_string;
    }
    return FAIL;
}

bool get_archs(const std::string& branch_name,
                   std::vector<std::string>& archs)
{
    const std::string get_url = "https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=";
    auto resp = curl_get(get_url + branch_name);
    if (resp == FAIL) {
        std::cout << "failed to get branch " << branch_name << std::endl;
        return false;
    }
    json json_archs = json::parse(resp)["archs"];
    std::transform(json_archs.cbegin(), json_archs.cend(), std::back_inserter(archs), 
        [](const json& object) { return object["arch"]; });
    return true;
}

void sort_by_name(json& packages)
{
    std::sort(packages.begin(), packages.end(),
        [](const auto& lhs, const auto& rhs)
        { return lhs["name"] < rhs["name"]; });
}

json get_branch(const std::string& branch_name,
                    const std::string& arch)
{
    const std::string get_url = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    auto response = curl_get(get_url + branch_name + "?arch=" + arch);
    if (response == FAIL) {
        return nullptr;
    }
    auto object = json::parse(response);
    auto packages = object["packages"];
    sort_by_name(packages);
    return packages;
}


// найти пакеты, которые есть в 1-й ветке, но не в 2-й
json compare_branches(const json& branch0, const json& branch1)
{
    auto ret = json::array();
    auto it1 = branch1.cbegin();
    for (auto it0 = branch0.cbegin(); it0 != branch0.cend(); ++it0) {
        while (it1 != branch1.cend() && (*it1)["name"] < (*it0)["name"])
            ++it1;
        if (it1 != branch1.cend() && (*it1)["name"] == (*it0)["name"])
            continue;
        else
            ret.emplace_back(*it0);
    }
    return ret;
}

std::vector<int> parse_version(const std::string& input)
{
    std::vector<int> result;
    std::istringstream parser(input);
    int i;
    while (parser >> i) {
        result.push_back(i);
        int c = parser.get(); //Skip period
        if (std::islower(c)) { // letter at the end (e.g. 4.2a)
            result.push_back(c - 'a');
            return result;
        }
    }
    return result;
}

bool less_than_version(const std::string& lhs, const std::string& rhs)
{
    auto parsed_lhs = parse_version(lhs);
    auto parsed_rhs = parse_version(rhs);
    return std::lexicographical_compare(parsed_lhs.cbegin(), parsed_lhs.cend(),
                                        parsed_rhs.cbegin(), parsed_rhs.cend());
}

json compare_versions(const json& branch0, const json& branch1)
{
    auto ret = json::array();
    auto it1 = branch1.cbegin();
    for (auto it0 = branch0.cbegin(); it0 != branch0.cend(); ++it0) {
        while (it1 != branch1.cend() && (*it1)["name"] < (*it0)["name"])
            ++it1;
        if (it1 != branch1.cend() && (*it0)["name"] == (*it1)["name"]
                && less_than_version((*it1)["version"], (*it0)["version"])) {
            auto& element = ret.emplace_back(*it0);
            element["ver. in 2nd branch"] = (*it1)["version"];
        }
    }
    return ret;
}