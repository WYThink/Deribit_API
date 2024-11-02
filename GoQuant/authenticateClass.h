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
    //Client ID & Secret
    const std::string *clientID, *clientSecret;

    //Authorization Header
    std::string authorizationHeader{};

    //Currency Buffer
    std::vector<std::string> currencyBuffer{};

    //Kind Buffer
    std::vector<std::string> kindBuffer{ "future" , "option" , "spot" , "future_combo" , "option_combo" };

    //JSON String (Which will be sent to Server)
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
    //CURL Request
    void curlRequest(std::string* readBuffer , const std::string& jsonString , const std::string& curlURL , CURLcode *res);

    //Free CURL Headers
    void freeCURLHeaders();

    //Fill Currency Buffer
    void fillCurrencyBuffer();

    //Get Access Token From Server
    void getAccessTokenFromServer();

    //Callback (make it static)
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

    //Get Instrument
    void getInstrumentFromAPI();

    //Get Order Book
    void getOrderBookFromAPI();

    //Get Currencies
    void getCurrenciesFromAPI();

    //Buy Instrument
    void buyOrderForInstrument();

    //Get Current Positions
    void getCurrentPositions();

    //Constructor
    authenticateClass(CURL* obj, infoGather* sharedObj, const std::string* cliID, const std::string* cliSecret);

    //Destructor
    ~authenticateClass();
};

#endif // __AUTHCLASS__
