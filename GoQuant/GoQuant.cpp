#include <iostream>
#include "curl/curl.h"
#include "InfoGather.h"
#include "requestClass.h"

int main()
{
	//CURL_GLOBAL_INIT()
	curl_global_init(CURL_GLOBAL_ALL);

	//CURL Handle
	CURL* curl;

	//Curl easy initialization
	curl = curl_easy_init();

	//Humar ReadAble Error Buffer
	char buffer[2048]{};

	if (curl)
	{
		//CURL url setup
		curl_easy_setopt(curl, CURLOPT_URL, "https://www.deribit.com/api/v2/public/auth");

		//CURL Human Readable Error Buffer
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, buffer);

		//InfoGather Object
		infoGather* obj1 = new infoGather();
		std::cout << "Client Secret Key : " << obj1->retClientSecret() << '\n';
		std::cout << "Client ID : " << obj1->retClientID() << '\n';

		//requestClass Object
		requestClass* obj2 = new requestClass();

		//Calling bse64StringConstruct()
		obj2->bse64StringConstruct(obj1->retClientID(), obj1->retClientSecret());
		std::cout << "Encoded String : " << obj2->encoded_BSE64_String() << '\n';
		std::cout << "Decoded String : " << obj2->decoded_BSE64_String() << '\n';

		//Destroy Object
		delete obj1;
		delete obj2;
	}

	else
	{
		std::cout << "Unable to SetUp Curl HANDLE" << '\n';
		return 0;
	}

	return 0;
}
