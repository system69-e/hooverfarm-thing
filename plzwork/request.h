#pragma once
#define CURL_STATICLIB

#include <string>
#include "curl/curl.h"

struct Response 
{
    CURLcode code;
    std::string data;
    std::string headers;
};

class Request {

public:
    Request(std::string url) : url(url), data(""), headers(nullptr) {}

    Request(std::string url, std::string data) : url(url), data(data), headers(nullptr) {}

    Request(std::string url, std::string data, curl_slist* headers) : url(url), data(data), headers(headers)  {}

    ~Request() 
    {
        if(headers) curl_slist_free_all(headers);
        if(curl) curl_easy_cleanup(curl);
    }

    int initalize();
    Response post();
    Response get();
    Response Delete();
	
private:
    std::string url;
    std::string data;
    curl_slist* headers;

    void prepare();
    Response execute();

    CURL* curl;

};