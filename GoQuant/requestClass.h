#pragma once

#ifndef __REQUESTCLASS__
#define __REQUESTCLASS__

#include <iostream>
#include "InfoGather.h"
#include "base64.h"

class requestClass
{
private:
	//Storing Encoded ID & Secret
	std::string bse64IDSecret{};

public:
	//bse64 String Construct
	void bse64StringConstruct(std::string_view clientID, std::string_view clientSecret);

	//Return base64 Encoded ClientID & Client Secret String
	const std::string encoded_BSE64_String();

	//Return base64 Decoded ClientID & Client Secret String
	const std::string decoded_BSE64_String();
	
	//Constructor
	requestClass();

	//Destructor
	~requestClass();
};

#endif // __REQUESTCLASS__