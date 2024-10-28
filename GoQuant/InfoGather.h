#pragma once

#ifndef _-INFOGATHER__
#define _-INFOGATHER__

#include <iostream>

class infoGather
{
private:
	//API Key
	std::string apiKey{};

	//UserName & Password
	std::string clientID{};

	//Signature
	const std::string *sigNature;

public:

	//Get API Key
	void getAPIKey();

	//Constructor
	infoGather();

	//Destructor
	~infoGather();

};

#endif // _INFOGATHER_