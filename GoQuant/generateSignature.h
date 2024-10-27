#pragma once

#ifndef __GENSIGNATURE__
#define __GENSIGNATURE__

#include <iostream>
#include <random>
#include <sstream>

class genSignature
{
private:
	//Signature
	std::string sigNature{};

public:
	//Generate Nonce
	std::string generateNonce();

	//Generate TimeStamp
	std::string getCurrentTimestamp();

	//Create JSON Body
	std::string createBody(int amount, int price);
	
	//Constructor
	genSignature();

	//Destructor
	~genSignature();
};

#endif // __GENSIGNATURE__