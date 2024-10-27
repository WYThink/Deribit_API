#pragma once

#ifndef _INFOGATHER_
#define _INFOGATHER_

#include <iostream>

class infoGather
{
private:
	//API Key
	std::string apiKey;

	//UserName & Password
	std::string userName , password;

public:

	//Get API Key
	void getAPIKey();

	//Get Username & Password
	void getUserPass();

	//Constructor
	infoGather();

	//Destructor
	~infoGather();

};

#endif // _INFOGATHER_