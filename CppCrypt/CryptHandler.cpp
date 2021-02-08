#include "CryptHandler.h"

CryptHandler::~CryptHandler()
{
}

void CryptHandler::setHandler(CryptHandler* s)
{
	successor = s;
}

uint8_t* CryptHandler::handleRequestDecrypt()
{
	if (successor != 0) {
		successor->handleRequestDecrypt();
	}

	return nullptr;
}

uint8_t* CryptHandler::handleRequestEncrypt()
{
	if (successor != 0) {
		successor->handleRequestEncrypt();
	}

	return nullptr;
}

