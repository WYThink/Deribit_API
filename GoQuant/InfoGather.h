#pragma once

#ifndef __INFOGATHER__
#define __INFOGATHER__

#include <iostream>

class infoGather
{
private:
	//API Key
	std::string apiKey{};

	//Client ID
	std::string clientID{};

	//Signature
	const std::string *sigNature;

public:

	//Get API Key
	void getAPIKey();

	//Return API Key
	std::string_view retApiKey();

	//Return Client ID
	std::string_view retClientID();

	//Constructor
	infoGather();

	//Destructor
	~infoGather();

};

#endif // _INFOGATHER_