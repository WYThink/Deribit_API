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

	//Curl easy initialization
	curl = curl_easy_init();

	if (curl)
	{
		//InfoGather Object
		infoGather* obj1 = new infoGather();

		//requestClass Object
		authenticateClass* obj2 = new authenticateClass(curl , obj1 , obj1->retAddrclientID() , obj1->retAddrclientSecret());
		
		//Get Instrument Name
		obj2->getInstrumentFromAPI();
		obj2->getOrderBookFromAPI();

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
