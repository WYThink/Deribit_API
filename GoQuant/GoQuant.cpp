#include <iostream>
#include "inputValidator.h"
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

	//Check For User Input
	bool check = true;

	if (curl)
	{
		//InfoGather Object
		infoGather* obj1 = new infoGather();

		//requestClass Object
		authenticateClass* obj2 = new authenticateClass(curl , obj1 , obj1->retAddrclientID() , obj1->retAddrclientSecret());
		
		//User Choice
		int choice{};

		while (check)
		{
			std::cout << std::endl;
			std::cout << "Welcome to the Trading System!\n";
			std::cout << "Please choose an option from the menu below:\n";
			std::cout << "1) Display available currencies\n";
			std::cout << "2) Display available instruments\n";
			std::cout << "3) Place an order\n";
			std::cout << "4) Show Bids\n";
			std::cout << "5) View Current Positions\n";
			std::cout << "6) Exit\n";
			std::cout << "Enter your choice (1-6): ";

			std::cin >> choice;
			if (std::cin.fail())
			{
				std::cin.clear();
				ignoreLine();
				std::cout << "Invalid Input. Please Enter Again" << std::endl;
			}

			switch (choice)
			{
				case 1:
					//Get Currencies
					obj2->getCurrenciesFromAPI();
					break;

				case 2:
					//Get Instrument Name
					obj2->getInstrumentFromAPI();
					break;

				case 3:
					//Buy Instrument
					obj2->buyOrderForInstrument();
					break;

				case 4:
					//Get Order Book
					obj2->getOrderBookFromAPI();
					break;
				
				case 5:
					//View Current Positions
					obj2->getCurrentPositions();
					break;

				case 6:
					//Exit The Program
					std::cout << "Exiting the system. Thank you!\n";
					check = false;
					break;

				default:
					std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
					break;
			}
		}

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
