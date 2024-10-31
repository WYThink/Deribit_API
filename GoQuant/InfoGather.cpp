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

//Return Address of clientID
const std::string* infoGather::retAddrclientID()
{
	return &clientID;
}

//Return Address of clientSecret
const std::string* infoGather::retAddrclientSecret()
{
	return &clientSecret;
}

//Destructor
infoGather::~infoGather() {}