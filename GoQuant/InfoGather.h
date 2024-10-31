#pragma once

#ifndef __INFOGATHER__
#define __INFOGATHER__

#include <iostream>

class infoGather
{
private:
	//API Key
	std::string clientSecret{};

	//Client ID
	std::string clientID{};

	//Signature
	const std::string *sigNature;

public:

	//Get API Key
	void getAPIKey();

	//Return API Key
	std::string retClientSecret();

	//Return Client ID
	std::string retClientID();

	//Return Address of clientID
	const std::string* retAddrclientID();
	
	//Return Address of clientSecret
	const std::string* retAddrclientSecret();

	//Constructor
	infoGather();

	//Destructor
	~infoGather();

};

#endif // _INFOGATHER_