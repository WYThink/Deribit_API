#include <iostream>
#include "InfoGather.h"

int main()
{
	//InfoGather Object
	infoGather* obj1 = new infoGather();
	std::cout << "API Key : " << obj1->retApiKey() << '\n';
	std::cout << "Client ID : " << obj1->retClientID() << '\n';

	//Destroy Object
	delete obj1;

	return 0;
}
