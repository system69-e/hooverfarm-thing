#include "roblox.h"
#include "request.h"

std::string findHeader(std::string headers, std::string toFind) {
    std::string header = "";
    size_t pos = headers.find(toFind);
    if (pos != std::string::npos) {
        pos = headers.find(":", pos);
        pos += 2; // skip ': '
        size_t end = headers.find("\r", pos);
        header = headers.substr(pos, end - pos);
    }
    return header;
}

std::string getRobloxTicket(std::string cookie) {
    // first we need to get the csrf token
	curl_slist* headers = curl_slist_append(nullptr, ("Cookie: .ROBLOSECURITY=" + cookie).c_str());
    headers = curl_slist_append(headers, "Referer: https://www.roblox.com/"); // tell request that we are from roblox

    Request req("https://auth.roblox.com/v1/authentication-ticket", "", headers);
    req.initalize();
    Response res = req.post();// This will fail but it will gives us the csrf token
    
    // now we need to parse the headers to get the csrf token
    std::string csrfToken = findHeader(res.headers, "x-csrf-token");

    // now we repeat the same process but this time we will get the auth ticket

    // modify the headers to include the csrf token
    headers = curl_slist_append(headers, ("x-csrf-token: " + csrfToken).c_str());

    // repeat the request
    res = req.post();

    // parse the headers to get the auth ticket
    std::string ticket = findHeader(res.headers, "rbx-authentication-ticket");
    return ticket;
}