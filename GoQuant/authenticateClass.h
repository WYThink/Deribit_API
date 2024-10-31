#pragma once
#ifndef __AUTHCLASS__
#define __AUTHCLASS__

#include <iostream>
#include <nlohmann/json.hpp>
#include "curl/curl.h"

class authenticateClass
{
private:
	//Client ID & Secret
	std::string clientID{}, clientSecret{};

	//Authorization Header
	std::string authorizationHeader{};

	//JSON String
	std::string jsonString{};

	//Pointer to Curl
	CURL* curl;

	//Headers Struct
	struct curl_slist* headers{ NULL };

public:
	//Get ClientID & ClientSecret
	void getClientIDSecret();

	//Get Access Token From Server
	void getAccessTokenFromServer();

	//CallBack
	size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {};

	//Constructor
	authenticateClass(CURL* obj , const std::string* cliID , const std::string* cliSecret);

	//Destructor
	~authenticateClass();
};

#endif // __AUTHCLASS__