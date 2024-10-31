#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "authenticateClass.h"
#include "curl/curl.h"

// Alias
using json = nlohmann::json;

// Get Access Token From Server
void authenticateClass::getAccessTokenFromServer()
{
    // CURL Status Code
    CURLcode res;

    // Read Buffer
    std::string readBuffer{};

    //Temp JSON Object
    json tmpObj;

    // Create JSON Payload
    json root;
    json params;

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9929;
    root["method"] = "public/auth";

    params["grant_type"] = "client_credentials";
    params["client_id"] = clientID;
    params["client_secret"] = clientSecret;
    root["params"] = params;

    // JSON String
    jsonString = root.dump();

    //try and catch block
    try
    {
        //CURL Setup
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.deribit.com/api/v2/public/auth");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Headers
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // JSON String as Post Data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

        // Setup Callback Function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the Request
        res = curl_easy_perform(curl);
    }
    catch (std::exception e)
    {
        std::cout << "Error : " << e.what() << '\n';
    }

    // Check for Errors
    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() Failed : " << curl_easy_strerror(res) << std::endl;
    }
    else
    {
        //Convert To JSON Object
        tmpObj = json::parse(readBuffer);

        //Store the Response
        jsonResponse = tmpObj.dump();

        //Print Response
        std::cout << "Response : " << tmpObj.dump(2) << std::endl;
    }
}

// Callback (made static)
size_t authenticateClass::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Constructor
authenticateClass::authenticateClass(CURL* obj, const std::string* cliID, const std::string* cliSecret) :
    curl{ obj },
    clientID{ *cliID },
    clientSecret{ *cliSecret }
{
    // Calling getAccessTokenFromServer()
    getAccessTokenFromServer();
}

// Destructor
authenticateClass::~authenticateClass()
{
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}
