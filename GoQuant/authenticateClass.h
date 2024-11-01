#pragma once
#ifndef __AUTHCLASS__
#define __AUTHCLASS__

#include <iostream>
#include <nlohmann/json.hpp>
#include "InfoGather.h"
#include "curl/curl.h"

class authenticateClass
{
private:
    // Client ID & Secret
    const std::string *clientID, *clientSecret;

    // Authorization Header
    std::string authorizationHeader{};

    // JSON String (Which will be sent to Server)
    std::string jsonString{};

    //JSON Response (Which Will be Receivded from the Server)
    std::string jsonResponse{};

    //Access Token
    std::string accessToken{};

    // Pointer to Curl
    CURL* curl;

    // Headers Struct
    struct curl_slist* headers{};

    //infoGather Object
    infoGather* infoGatherSharedObject{NULL};

public:
    // Get Access Token From Server
    void getAccessTokenFromServer();

    // Callback (make it static)
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

    // Get Instrument
    void getInstrumentFromAPI();

    // Get Order Book
    void getOrderBookFromAPI();

    // Constructor
    authenticateClass(CURL* obj, infoGather* sharedObj, const std::string* cliID, const std::string* cliSecret);

    // Destructor
    ~authenticateClass();
};

#endif // __AUTHCLASS__
