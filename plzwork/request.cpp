#include "request.h"

static size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) 
{
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int Request::initalize() 
{
    CURL* localCurl = curl_easy_init();
    this->curl = localCurl;
    if (!this->curl) {
        return 1;
    }
    return 0;
}

void Request::prepare() {
#ifdef _DEBUG
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // set header if there is one
    if(headers) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
}

Response Request::get() 
{
    this->prepare();
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    return this->execute();   
}

Response Request::post() 
{
    this->prepare();
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    return this->execute();
}
/*
Response Request::Delete()
{
    this->prepare();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	return this->execute();
}
*/

Response Request::execute() 
{
    Response response{};
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.data);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.headers);

    CURLcode code = curl_easy_perform(curl);
    response.code = code;
    return response;
}