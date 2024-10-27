#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <random>
#include <string>
#include <openssl/hmac.h>
#include "generateSignature.h"

//Constructor
genSignature::genSignature() {}

//Generate Nonce
std::string genSignature::generateNonce() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    std::ostringstream oss;
    oss << dis(gen);
    return oss.str();
}

//Generate TimeStamp
std::string genSignature::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* gmtime = std::gmtime(&now);
    std::ostringstream oss;
    oss << std::put_time(gmtime, "%Y%m%dT%H%M%SZ");
    return oss.str();
}

//Create JSON Body
std::string genSignature::createBody(int amount, int price) {
    std::string body = "{\"amount\":" + std::to_string(amount) + ",\"price\":" + std::to_string(price) + "}";
    return body;
}

//Destructor
genSignature::~genSignature() {}