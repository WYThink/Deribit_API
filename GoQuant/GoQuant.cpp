#include <iostream>
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include "InfoGather.h"
#include "requestClass.h"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
	size_t totalSize = size * nmemb;
	response->append((char*)contents, totalSize);
	return totalSize;
}

int main()
{
	//Global Preparation , calling CURL_GLOBAL_INIT() Function
	curl_global_init(CURL_GLOBAL_ALL);

	//CURL Handle
	CURL* curl;
	std::string response;

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

		//Authorization Header String
		const std::string& authoSTR = obj2->retAuthorizationHeader();

		std::cout << authoSTR << '\n';

		//CURL HTTP Header & Calling slist_append() to append Header
		struct curl_slist* headers{ NULL };
		headers = curl_slist_append(headers, authoSTR.c_str());

		//CURL POSTFIELDS
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		//Perform Requeste
		CURLcode res;
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
		}
		else
		{
			std::cout << "Authentication request sent successfully!" << std::endl;

			try {
				json jsonResponse = json::parse(response);
				std::string accessToken = jsonResponse["result"]["access_token"];
				std::cout << "Access Token: " << accessToken << std::endl;
			}
			catch (json::exception& e) {
				std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
			}
		}

		//Destroy Object
		delete obj1;
		delete obj2;

		//CURL CleanUp
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
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
