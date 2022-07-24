#include "utils.h"

#include <curl/curl.h>
#include <string>
#include <iostream>

size_t write_function(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

std::string get_branch(const std::string& branch_name)
{
    auto curl = curl_easy_init();
    if (curl) {
        std::string get_url = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch_name;
        curl_easy_setopt(curl, CURLOPT_URL, get_url.c_str());
        
        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl = NULL;

        return response_string;
    }
    return "";
}