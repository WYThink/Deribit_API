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

	//calling getUserPass()
	getUserPass();
}

//Get API Key
void infoGather::getAPIKey()
{
	//API Key Input
	std::cout << "Enter your API Key: ";
	std::getline(std::cin >> std::ws, apiKey);

	//If Failed Take Input Again
	getAPIKey();
}

//Get Username & Password
void infoGather::getUserPass()
{
	//Get UserName
	std::cout << "Enter your UserName: ";
	std::getline(std::cin >> std::ws, userName);

	//Get Password
	std::cout << "Enter your Password: ";
	std::getline(std::cin >> std::ws, password);

	//If Failed Take Input Again
	getUserPass();
}

//Destructor
infoGather::~infoGather() {}