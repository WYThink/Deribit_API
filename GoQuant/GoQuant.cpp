#include <iostream>
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include "InfoGather.h"
#include "authenticateClass.h"

int main()
{
	//Global Preparation , calling CURL_GLOBAL_INIT() Function
	curl_global_init(CURL_GLOBAL_ALL);

	//CURL Handle
	CURL* curl;
	std::string response;

	//Curl easy initialization
	curl = curl_easy_init();

	if (curl)
	{
		//InfoGather Object
		infoGather* obj1 = new infoGather();
		std::cout << "Client Secret Key : " << obj1->retClientSecret() << '\n';
		std::cout << "Client ID : " << obj1->retClientID() << '\n';

		//requestClass Object
		authenticateClass* obj2 = new authenticateClass(curl , obj1->retAddrclientID() , obj1->retAddrclientSecret());

		//Destroy Object
		delete obj1;
		delete obj2;
	}

	else
	{
		std::cout << "Unable to SetUp Curl HANDLE" << '\n';
		return 0;
	}

	//CURL Global CleanUp
	curl_global_cleanup();

	return 0;
}
