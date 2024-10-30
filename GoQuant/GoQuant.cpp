#include <iostream>
#include "InfoGather.h"
#include "requestClass.h"

int main()
{
	//InfoGather Object
	infoGather* obj1 = new infoGather();
	std::cout << "Client Secret Key : " << obj1->retClientSecret() << '\n';
	std::cout << "Client ID : " << obj1->retClientID() << '\n';

	//requestClass Object
	requestClass* obj2 = new requestClass();

	//Calling bse64StringConstruct()
	obj2->bse64StringConstruct(obj1->retClientID() , obj1->retClientSecret());
	std::cout << "Encoded String : " << obj2->encoded_BSE64_String() << '\n';
	std::cout << "Decoded String : " << obj2->decoded_BSE64_String() << '\n';

	//Destroy Object
	delete obj1;
	delete obj2;

	return 0;
}
