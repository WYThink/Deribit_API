#include "requestClass.h"
#include "base64.hpp"

//Constructor
requestClass::requestClass() {}

//bse64 String Construct
void requestClass::bse64StringConstruct(std::string_view clientID, std::string_view clientSecret)
{
	//Concatenate ID & Secret
	auto id_secret = (std::string)clientID + ":" + (std::string)clientSecret;

	//Base64 Encode
	std::string bse64STR = base64::to_base64(id_secret);
	
	//Setting Value
	bse64IDSecret = bse64STR;

	//Setting Authorization Header
	authorizationHeader = "Authorization: " + bse64IDSecret;
}

//Return Encoded String
const std::string requestClass::encoded_BSE64_String()
{
	return bse64IDSecret;
}

//Return Decode String
const std::string requestClass::decoded_BSE64_String()
{
	return (std::string)base64::from_base64(bse64IDSecret);
}

//Return Authorization Header
const std::string requestClass::retAuthorizationHeader()
{
	return authorizationHeader;
}

//Destructor
requestClass::~requestClass()
{
	//Setting to Default
	bse64IDSecret = "";
}