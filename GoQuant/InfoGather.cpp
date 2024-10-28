#include <iostream>
#include <string>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include "InfoGather.h"

//Constructor
infoGather::infoGather()
{
	//calling getAPIKey()
	getAPIKey();
}

//Get API Key
void infoGather::getAPIKey()
{
	//API Key Input
	std::cout << "Enter API Key: ";
	std::getline(std::cin >> std::ws, apiKey);

	//Client ID
	std::cout << "Enter Client ID: ";
	std::getline(std::cin >> std::ws, clientID);

	//If Failed Take Input Again
	getAPIKey();
}

//Destructor
infoGather::~infoGather() {}