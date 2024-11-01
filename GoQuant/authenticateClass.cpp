#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "authenticateClass.h"
#include "curl/curl.h"

// Alias
using json = nlohmann::json;

// Callback Function
size_t authenticateClass::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

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
    json root , params;

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "public/auth";

    params["grant_type"] = "client_credentials";
    params["client_id"] = *clientID;
    params["client_secret"] = *clientSecret;
    root["params"] = params;

    // JSON String
    jsonString = root.dump();

    //try and catch block
    try
    {
        // Headers
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        // CURL Setup
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, "https://test.deribit.com/api/v2/public/auth");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        //CallBack Function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        //Specifing Buffer
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

        //Check for "Invalid Credentials"
        if (tmpObj.contains("error") && tmpObj["error"].contains("code") && tmpObj["error"].contains("message")) {

            //Error Code
            std::string errorCode{};

            //Check if code is a String or Number
            if (tmpObj["error"]["code"].is_number())
            {
                errorCode = std::to_string(tmpObj["error"]["code"].get<int>());
            }
            else if (tmpObj["error"]["code"].is_string())
            {
                errorCode = tmpObj["error"]["code"].get<std::string>();
            }

            //Check for Invalid Credentials
            if (errorCode == "13004" && "invalid_credentials" == tmpObj["error"]["message"].get<std::string>())
            {
                std::cout << "Invalid Credentials!! Please try again" << std::endl;

                // Call getAPIKey() for New Input
                infoGatherSharedObject->getAPIKey();
                
                //Call getAccessTokenFromServer()
                getAccessTokenFromServer();
            }
        }

        //Store the Response
        jsonResponse = tmpObj.dump();

        //Store & Print Access Token
        accessToken = tmpObj["result"]["access_token"].get<std::string>();
        std::cout << "Access Token : " << accessToken << '\n';

        //Reset Feature
        if (headers)
        {
            //Reset Headers
            curl_slist_free_all(headers);
            headers = nullptr;
        }
    }
}

//Get Instrument From API
void authenticateClass::getInstrumentFromAPI()
{
    //CURL Status Code
    CURLcode res;

    //Read Buffer
    std::string readBuffer{};

    //Reset headers
    if (headers)
    {
        curl_slist_free_all(headers);
    }

    // Create JSON Payload
    json root;

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "public/get_instruments";
    
    root["params"] = {
        {"currency", "ETH"},
        {"kind", "spot"}
    };

    // Convert JSON to string
    std::string jsonData = root.dump();

    // Headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authoHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authoHeader.c_str());

    //CURL Field Setup
    curl_easy_setopt(curl , CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl , CURLOPT_URL, "https://test.deribit.com/api/v2/public/get_instruments");
    curl_easy_setopt(curl , CURLOPT_POSTFIELDS, jsonData.c_str());
    
    //CallBack Function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    //Specifing Buffer
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    //Perform Operation
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() Failed: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        // Parse the response
        json response = json::parse(readBuffer);

        // Check if 'result' is present in the response
        if (response.contains("result")) {
            std::cout << std::endl <<"<------------------ Instrument Name ------------------>" << std::endl;
            for (const auto& instrument : response["result"]) {
                std::cout << "Instrument Name: " << instrument["instrument_name"] << std::endl;
            }
        }
        else {
            std::cerr << "No result found in the response." << std::endl;
        }
    }

    if (headers)
    {
        // Free the headers list after the request
        curl_slist_free_all(headers);
        headers = nullptr;
    }
}

// Get Order Book
void authenticateClass::getOrderBookFromAPI()
{
    //CURL Status Code
    CURLcode res;

    //Read Buffer
    std::string readBuffer{};

    //Reset Header
    if (headers)
    {
        curl_slist_free_all(headers);
        headers = nullptr;
    }

    //JSON Object
    json root;

    //JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "/public/get_order_book";

    root["params"] = {
        {"instrument_name" , "BTC-PERPETUAL"},
        {"depth" , 5}
    };

    //JSON to String
    std::string jsonData = root.dump();

    //Headers
    headers = curl_slist_append(headers , "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    //CURL Field Setup
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, "https://test.deribit.com/api/v2/public/get_order_book");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    
    //CallBack Function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    //Specifing Buffer
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    //Perform Request
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() Failed" << curl_easy_strerror(res) << std::endl;
    }
    else
    {
        try
        {
            //Parse The Response
            json response = json::parse(readBuffer);

            // Check if 'result' is present in the response
            if (response.contains("result") && response["result"].contains("bids")) {
                std::cout << std::endl << "<----------------- Bids ----------------->" << std::endl;

                // Iterate over each bid in the "bids" array
                for (const auto& bid : response["result"]["bids"]) {
                    if (bid.size() == 2) {
                        double price = bid[0];
                        double quantity = bid[1];
                        std::cout << "Price: " << price << "\t, Quantity: " << quantity << std::endl;
                    }
                }
            }
            else {
                std::cout << "'bids' not found in the response." << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error : " << e.what() << '\n';
        }
    }

    //Reset Headers
    if (headers)
    {
        //Free the Header List
        curl_slist_free_all(headers);
        headers = nullptr;
    }
}

// Get Currencies
void authenticateClass::getCurrenciesFromAPI()
{
    //CURL Status Code
    CURLcode res;

    //Read Buffer
    std::string readBuffer{};

    //Reset Header
    if (headers)
    {
        //Free The Headers List
        curl_slist_free_all(headers);
    }

    //JSON Object
    json root;

    //JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "/public/get_currencies";

    //JSON to String
    std::string jsonData = root.dump();

    //CURL Header Setup
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    //CURL Field Setup
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, "https://test.deribit.com/api/v2/public/get_currencies");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

    //CallBack Function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    
    //Specifing Buffer
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    try
    {
        //Perform Request
        res = curl_easy_perform(curl);
    }
    catch (const std::exception& e)
    {
        std::cout << "Error : " << e.what() << '\n';
    }

    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() Failed" << curl_easy_strerror(res) << std::endl;
    }
    else
    {
        try
        {
            //Parsing JSON Type Response
            json response = json::parse(readBuffer);

            //Print Response
            std::cout << response.dump(3) << std::endl;

            //Total Currencies
            int totalCurrency{};

            if (response.contains("result"))
            {
                std::cout << std::endl << "<----------------- Currencies ----------------->" << std::endl;
                for (const auto& item : response["result"])
                {
                    std::cout << "Currency \t: " << item["currency"] << std::endl;
                    std::cout << "Currency Long \t: " << item["currency_long"] << std::endl;
                    std::cout << std::endl;

                    totalCurrency += 1;
                }
                std::cout << "Total Currency : " << totalCurrency << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error : " << e.what() << '\n';
        }
    }

    //Reset Headers
    if (headers)
    {
        //Free Headers List
        curl_slist_free_all(headers);
        headers = nullptr;
    }
}

// Constructor
authenticateClass::authenticateClass(CURL* obj , infoGather* sharedObj, const std::string* cliID, const std::string* cliSecret) :
    curl{ obj },
    infoGatherSharedObject{sharedObj},
    clientID{ cliID },
    clientSecret{ cliSecret }
{
    // Calling getAccessTokenFromServer()
    getAccessTokenFromServer();
}

// Destructor
authenticateClass::~authenticateClass()
{
    //Clean Up
    curl_easy_cleanup(curl);
}
