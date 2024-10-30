#include <iostream>
#include <string>
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
	std::cout << "Enter Client Secret: ";
	std::getline(std::cin >> std::ws, clientSecret);

	//Client ID
	std::cout << "Enter Client ID: ";
	std::getline(std::cin >> std::ws, clientID);

	//If Failed Take Input Again
	//getAPIKey();
}

//Return API Key
std::string infoGather::retClientSecret()
{
	return clientSecret;
}

//Return Client ID
std::string infoGather::retClientID()
{
	return clientID;
}

//Destructor
infoGather::~infoGather() {}