#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <string>
#include <nlohmann/json.hpp>
#include "authenticateClass.h"
#include "curl/curl.h"

void ignoreInvalidInput();

// Alias
using json = nlohmann::json;

// Callback Function
size_t authenticateClass::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//Free CURL Headers
void authenticateClass::freeCURLHeaders()
{
    if (headers)
    {
        // Free the headers list after the request
        curl_slist_free_all(headers);
        headers = nullptr;
    }
}

//Fill Currency Buffer
void authenticateClass::fillCurrencyBuffer()
{
    //CURL Status Code
    CURLcode res;

    //Read Buffer
    std::string readBuffer{};

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

    //Calling curlRequest()
    curlRequest(&readBuffer, jsonData, "https://test.deribit.com/api/v2/public/get_currencies", &res);

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

            if (response.contains("result"))
            {
                for (const auto& item : response["result"])
                {
                    //Push Back Currency
                    currencyBuffer.push_back(item["currency"].get<std::string>());
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error : " << e.what() << '\n';
        }
    }

    //calling freeCURLHeaders()
    freeCURLHeaders();
}

//CURL Request
void authenticateClass::curlRequest(std::string* readBuffer, const std::string& jsonString, const std::string& curlURL, CURLcode* res )
{
    try
    {
        //CURL Setup
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, curlURL.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        
        //CallBack Function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, readBuffer);

        //Perform Requeset
        *res = curl_easy_perform(curl);
    }
    catch (const std::exception& e)
    {
        std::cout << "Error : " << e.what() << '\n';
    }
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
    json root ;

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "public/auth";

    root["params"] = {
        {"grant_type" , "client_credentials" },
        {"client_id" , *clientID} ,
        {"client_secret" , *clientSecret }
    };

    // JSON String
    jsonString = root.dump();

    // Headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    //Calling curlRequest()
    curlRequest(&readBuffer, jsonString, "https://test.deribit.com/api/v2/public/auth", &res);

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
        //std::cout << "Access Token : " << accessToken << '\n';

        //Reset Feature
        freeCURLHeaders();
    }
}

//Get Instrument From API
void authenticateClass::getInstrumentFromAPI()
{
    //User Input
    std::string instrumentCodeInput{};

    //CURL Status Code
    CURLcode res;

    //Read Buffer
    std::string readBuffer{};

    // Create JSON Payload
    json root;

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "public/get_instruments";

    //Display Currencies
    std::cout << std::endl << "<------------------ Currencies ------------------>" << std::endl;
    for (const auto& currency : currencyBuffer)
    {
        std::cout << "Name : " << currency.c_str() << std::endl;
    }
    std::cout << std::endl;

    //Curreny Code Input
    while (true)
    {
        //Input
        std::cout << "Please enter the Instrument Code (e.g., ETHW, BTC), or type 'any' to see all available instrument codes : ";
        std::getline(std::cin >> std::ws , instrumentCodeInput);

        //Find inside the currencyBuffer
        auto it = std::find(currencyBuffer.begin(), currencyBuffer.end(), instrumentCodeInput);

        if (it != currencyBuffer.end())
        {
            root["params"] = {
                {"currency",instrumentCodeInput.c_str()}
            };
            break;
        }
        else if (instrumentCodeInput == "any")
        {
            root["params"] = {
                {"currency","any"}
            };
            break;
        }
        else
        {
            std::cout << "Invalid Input. Please Try Again" << std::endl;
        }
    }

    // Convert JSON to string
    std::string jsonData = root.dump();

    // Headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authoHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authoHeader.c_str());

    //Calling curlRequest()
    curlRequest(&readBuffer, jsonData, "https://test.deribit.com/api/v2/public/get_instruments", &res);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() Failed: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        // Parse the response
        json response = json::parse(readBuffer);

        // Check if 'result' is present in the response
        if (response.contains("result"))
        {
            if (response["result"].empty())
            {
                std::cout << std::endl << "No Instruments Currently Available for " << instrumentCodeInput.c_str() << std::endl;
                
                //calling freeCURLHeaders()
                freeCURLHeaders();
                return;
            }
            std::cout << std::endl << "<------------------ Instrument Name ------------------>" << std::endl;
            for (const auto& instrument : response["result"])
            {
                std::cout << "Instrument Name: " << instrument["instrument_name"] << std::endl;
            }
        }
        else
        {
            std::cerr << "No result found in the response." << std::endl;
        }
    }

    //Calling freeCURLHeaders()
    freeCURLHeaders();
}

// Get Order Book
void authenticateClass::getOrderBookFromAPI()
{
    //calling freeCURLHeaders()
    freeCURLHeaders();
    
    //CURL Status Code
    CURLcode res;

    //Read Buffer
    std::string readBuffer{};

    //JSON Object
    json root;

    //Instrumental Name
    std::string instrumentalName{};

    std::cout << "Please enter the instrument name (e.g., BTC_EURR): ";
    std::getline(std::cin >> std::ws, instrumentalName);

    //JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "/public/get_order_book";

    root["params"] = {
        {"instrument_name" , instrumentalName.c_str()},
        {"depth" , 5}
    };

    //JSON to String
    std::string jsonData = root.dump();

    //Headers
    headers = curl_slist_append(headers , "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    //Calling curlRequest()
    curlRequest(&readBuffer, jsonData, "https://test.deribit.com/api/v2/public/get_order_book", &res);

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
                for (const auto& bid : response["result"]["bids"]) 
                {
                    if (bid.size() == 2) {
                        double price = bid[0];
                        double quantity = bid[1];
                        std::cout << "Price: " << price << "\t, Quantity: " << quantity << std::endl;
                    }
                }
                std::cout << std::endl;
                std::cout << "Best Bid Price : " << response["result"]["best_bid_price"] << '\n';
                std::cout << "Best Bid Amount : " << response["result"]["best_bid_amount"] << '\n';
                std::cout << "Best Ask Price : " << response["result"]["best_ask_price"] << '\n';
                std::cout << "Best Ask Amount : " << response["result"]["best_ask_amount"] << '\n';

            }

            //Check if 'invalid instrument name' in the response
            else if (response.contains("error"))
            {
                //Check for Error Code & Invalid Parameter Name
                if (-32602 == response["error"]["code"].get<int>() && response["error"]["data"]["param"].get<std::string>() == "instrument_name")
                {
                    std::cout << "Invalid Instrument Name. Please try again" << std::endl;
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

    //calling freeCURLHeaders()
    freeCURLHeaders();
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

    //Calling curlRequest()
    curlRequest(&readBuffer, jsonData, "https://test.deribit.com/api/v2/public/get_currencies", &res);

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

            //Total Currencies
            int totalCurrency{};

            if (response.contains("result"))
            {
                std::cout << std::endl << "<----------------- Currencies ----------------->" << std::endl;
                for (const auto& item : response["result"])
                {
                    std::cout << "Name : " << item["currency_long"].get<std::string>() 
                        << ",(" << item["currency"].get<std::string>() << ")" << std::endl;

                    totalCurrency += 1;
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error : " << e.what() << '\n';
        }
    }

    //calling freeCURLHeaders()
    freeCURLHeaders();
}

// Buy Instrument
void authenticateClass::buyOrderForInstrument()
{
    // CURL Status Code
    CURLcode res;

    // Read Buffer
    std::string readBuffer{};

    // Input Instrument Name
    std::string instrumentNameInput{};
    std::cout << "Enter Instrument Name: ";
    std::getline(std::cin >> std::ws, instrumentNameInput);
    if (std::cin.fail()) ignoreInvalidInput();

    // Input Amount
    int amount{};
    std::cout << "Enter Amount: ";
    std::cin >> amount;
    if (std::cin.fail()) ignoreInvalidInput();

    std::cout << "\n<------------------ Input Type ------------------>\n";

    // Print Input Types
    for (const auto& input : typeBuffer) {
        std::cout << input << '\n';
    }
    std::cout << std::endl;

    // Input Type
    std::string typeInput{};
    std::cout << "Enter Type (e.g., future, option, spot): ";
    std::cin >> typeInput;
    if (std::cin.fail()) ignoreInvalidInput();

    // JSON Object
    json root;

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "/private/buy";
    root["params"] = {
        {"amount", amount},
        {"instrument_name", instrumentNameInput.c_str()},
        {"label", "market0511"},
        {"type", typeInput.c_str()}
    };

    // Convert JSON to String
    std::string jsonString = root.dump();

    // CURL Headers Setup
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    // Calling curlRequest()
    curlRequest(&readBuffer, jsonString, "https://test.deribit.com/api/v2/private/buy", &res);

    if (res != CURLE_OK) 
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    else 
    {
        // Convert Response to JSON Format
        json response = json::parse(readBuffer);

        // Check for Errors
        if (response.contains("error")) {
            if (response["error"]["data"]["param"] == "instrument_name") 
            {
                std::cout << "Invalid \"Instrument Name\". Please Try Again" << std::endl;
            }
            else if (response["error"]["data"]["param"] == "type") 
            {
                std::cout << "Invalid \"Type Name\". Please Try Again" << std::endl;
            }
            else if (response["error"]["data"]["param"] == "amount") {
                std::cout << "Invalid \"Amount\". Please Try Again" << std::endl;
            }
            else 
            {
                std::cout << "Error occurred while placing order. Please try again." << std::endl;
            }

            //Free Headers
            freeCURLHeaders();
            return;
        }

        // Check for Order Result
        if (response.contains("result")) {
            if (response["result"].contains("order") && response["result"]["order"]["order_state"] == "filled") 
            {
                std::cout << "\nOrder was successful and filled." << std::endl;
                std::cout << response["result"]["order"]["instrument_name"] << std::endl;
                std::cout << response["result"]["order"]["order_id"] << std::endl;
                std::cout << response["result"]["order"]["order_type"] << std::endl;
            }
            else 
            {
                std::cout << "Order was not successful or is in a different state." << std::endl;
            }
        }
    }

    // Calling freeCURLHeaders()
    freeCURLHeaders();
}


//Get Current Positions
void authenticateClass::getCurrentPositions()
{
    // CURL Status Code
    CURLcode res;

    // Read Buffer
    std::string readBuffer;

    // JSON Object for the request payload
    json root;

    //JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "/private/get_positions";

    // Display Available Currencies
    std::cout << "\n<------------------ Available Currencies ------------------>\n";
    for (const auto& currency : currencyBuffer) {
        std::cout << "Name: " << currency << std::endl;
    }
    std::cout << std::endl;

    // Input: Currency and Kind Type
    std::string currencyInput, kindInput;
    while (true)
    {
        std::cout << "Enter Currency (or type 'any' to see all available options): ";
        std::getline(std::cin >> std::ws, currencyInput);

        std::cout << "Enter Kind Type (e.g., future, option, spot): ";
        std::getline(std::cin >> std::ws, kindInput);

        // Validate inputs
        bool validCurrency = (std::find(currencyBuffer.begin(), currencyBuffer.end(), currencyInput) != currencyBuffer.end()) || (currencyInput == "any");
        bool validKind = (std::find(kindBuffer.begin(), kindBuffer.end(), kindInput) != kindBuffer.end());

        if (validCurrency && validKind) {
            // Set JSON params
            root["params"] = {
                {"currency", currencyInput == "any" ? "any" : currencyInput.c_str()},
                {"kind", kindInput.c_str()}
            };
            break;
        }
        else {
            if (!validCurrency && currencyInput != "any") {
                std::cout << "Invalid Currency. Please try again.\n";
            }
            if (!validKind) {
                std::cout << "Invalid Kind Type. Please try again.\n";
            }
        }
    }

    // Convert JSON object to a string
    std::string jsonString = root.dump();

    // CURL Headers Setup
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    // Call curlRequest
    curlRequest(&readBuffer, jsonString, "https://test.deribit.com/api/v2/private/get_positions", &res);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        // Parse and handle response
        json response = json::parse(readBuffer);

        if (response.contains("error")) 
        {
            if (response["error"]["message"].get<std::string>() == "Invalid params")
            {
                std::cout << std::endl << "Error Message : " << response["error"]["message"].get<std::string>().c_str() << std::endl;
                std::cout << "Error Reason : " << response["error"]["data"]["reason"].get<std::string>().c_str() << std::endl;
                std::cout << std::endl << "Please Try Again" << std::endl;
            }

        }
        else if (response.contains("result") && !response["result"].empty()) {
            std::cout << "Positions:\n" << response["result"].dump(3) << '\n';
        }
        else {
            std::cout << std::endl << "Currently No positions available." << std::endl;
        }
    }

    // Free CURL Headers
    freeCURLHeaders();
}

//Get Open Orders
bool authenticateClass::getOpenOrder()
{
    // CURL Status Code
    CURLcode res;

    // Read Buffer
    std::string readBuffer{};

    // JSON Object
    json root;

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;
    root["method"] = "/private/get_open_orders";
    root["params"] = {};

    // JSON to String
    std::string jsonString{ root.dump() };

    // CURL Headers Setup
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    // Calling curlRequest()
    curlRequest(&readBuffer, jsonString, "https://test.deribit.com/api/v2", &res);

    bool hasOpenOrders = false;

    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    else
    {
        // Convert Response to JSON Format
        json response = json::parse(readBuffer);

        // Check if there are any open orders
        if (response.contains("result") && !response["result"].empty())
        {
            hasOpenOrders = true;
        }
        else if (response.contains("error"))
        {
            std::cerr << "Error: " << response["error"].dump(3) << '\n';
        }
    }

    // Free headers 
    freeCURLHeaders();

    // Return whether there are open orders
    return hasOpenOrders;
}

//Sell Instrument
void authenticateClass::cancelAnOrder()
{
    // Check For Orders; if "false" exit function
    if (!getOpenOrder())
    {
        std::cout << std::endl << "No \"Open Orders\" to cancel.\n";
        return;
    }

    // CURL Status Code
    CURLcode res;

    // Read Buffer
    std::string readBuffer{}, orderIDInput{};

    // JSON Object
    json root;

    // User Choice
    int num{};

    // JSON Payload
    root["jsonrpc"] = "2.0";
    root["id"] = 9940;

    while (true)
    {
        std::cout << "You have open orders. Would you like to:\n";
        std::cout << "1. Cancel all open orders\n";
        std::cout << "2. Cancel a specific order by Order ID\n";
        std::cout << "Enter your choice (1 or 2): ";
        std::cin >> num;

        if (std::cin.fail()) ignoreInvalidInput();

        if (num == 1)
        {
            root["method"] = "/private/cancel_all";
            break;
        }
        if (num == 2)
        {
            std::cout << "Enter Order ID: ";
            std::cin >> orderIDInput;
            if (std::cin.fail()) ignoreInvalidInput();

            root["method"] = "/private/cancel";
            root["params"] = { {"order_id", orderIDInput.c_str()} };
            break;
        }
        else
        {
            std::cout << "Invalid input. Please enter 1 or 2.\n";
        }
    }

    // Convert JSON to String
    std::string jsonString = root.dump();

    // CURL Headers Setup
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + accessToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    // Calling curlRequest()
    curlRequest(&readBuffer, jsonString, "https://test.deribit.com/api/v2", &res);

    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    else
    {
        // Convert Response to JSON Format
        json response = json::parse(readBuffer);

        if (response.contains("result")) std::cout << "Response: " << response["result"].dump(3) << '\n';
        else if (response.contains("error")) std::cout << "Error: " << response["error"].dump(3) << '\n';
        else std::cout << "Unknown error occurred.\n";
    }

    // Free Headers
    freeCURLHeaders();
}



// Constructor
authenticateClass::authenticateClass(CURL* obj , infoGather* sharedObj, const std::string* cliID, const std::string* cliSecret) :
    curl{ obj },
    infoGatherSharedObject{sharedObj},
    clientID{ cliID },
    clientSecret{ cliSecret }
{
    //Calling getAccessTokenFromServer()
    getAccessTokenFromServer();

    //Calling fillCurrencyBuffer()
    fillCurrencyBuffer();
}

// Destructor
authenticateClass::~authenticateClass()
{
    //Clean Up
    curl_easy_cleanup(curl);
}
